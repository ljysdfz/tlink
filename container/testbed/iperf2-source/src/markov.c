/*---------------------------------------------------------------
 * Copyright (c) 2024
 * Broadcom Corporation
 * All Rights Reserved.
 *---------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 *
 * Redistributions of source code must retain the above
 * copyright notice, this list of conditions and
 * the following disclaimers.
 *
 *
 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimers in the documentation and/or other materials
 * provided with the distribution.
 *
 *
 * Neither the name of Broadcom Coporation,
 * nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without
 * specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ________________________________________________________________
 *
 * markov.c
 * Suppport for markov chains
 *
 * by Robert J. McMahon (rjmcmahon@rjmcmahon.com, bob.mcmahon@broadcom.com)
 * -------------------------------------------------------------------
 */
#include <math.h>
#include "headers.h"
#include "markov.h"

static inline char * deblank (char *str) {
    char *out = str, *put = str;
    for(; *str != '\0'; ++str) {
	if(*str != ' ') {
	    *put = *str;
	    put++;
	}
    }
    *put = '\0';
    return out;
}

void markov_graph_free (struct markov_graph *graph) {
    if (graph) {
	int ix;
	struct markov_entry **tmp;
	tmp = graph->entrys;
	for (ix = 0; ix < graph->count; ix++) {
	    free(tmp[ix]);
	}
	free(tmp);
	free(graph->braket_str);
	free(graph);
    }
}

void markov_graph_print(struct markov_graph *graph, char *prepend) {
    if (graph) {
	struct markov_entry **tmp;
	int ix, jx;
	tmp = graph->entrys;
	if (tmp) {
	    printf("%sMarkov chain: %s transitions: %" PRIdMAX " unknowns: %" PRIdMAX "\n", prepend, graph->braket_str, graph->transition_cnt, graph->unknown_cnt);
	    for (ix = 0; ix < graph->count; ix++) {
		printf("%s%d=", prepend, tmp[ix][0].node_len);
		uintmax_t pullcnt = 0;
		for (jx = 0; jx < graph->count; jx++) {
		    pullcnt += tmp[ix][jx].transition_cnt;
		}
		for (jx = 0; jx < graph->count; jx++) {
		    printf("%d(%d,%d)|%0.3f/%0.3f(%" PRIdMAX "/%0.3f) ", tmp[ix][jx].adjacent_node_len, ix, jx, tmp[ix][jx].prob, tmp[ix][jx].prob_cummulative, tmp[ix][jx].transition_cnt, (float)tmp[ix][jx].transition_cnt / (float)pullcnt);
		}
		printf(" %" PRIdMAX "/%" PRIdMAX "(%0.1f%%)\n", pullcnt, graph->transition_cnt, 100.0 * (float) pullcnt / (float) graph->transition_cnt);
	    }
	}
    }
}

struct markov_graph *markov_graph_init (char *braket_option) {
    struct markov_graph *graph = calloc(1, sizeof(struct markov_graph));
    char *tmp_bra = malloc(strlen(braket_option) + 1);
    graph->braket_str = malloc(strlen(braket_option) + 1);
    strcpy(tmp_bra, braket_option);
    strcpy(graph->braket_str, braket_option);
    tmp_bra = deblank(tmp_bra);
    // printf("***braket:%s\n", tmp_bra);
    int bracnt = 0;

    char *bra_next;
    char *found;
    found = strtok(tmp_bra, "<");
    bra_next = found;
    while (found != NULL) {
	found = strtok(NULL,"<");
	bracnt++;
    }
    int count = bracnt;
    if (graph && (bracnt > 0)) {
	int ix;
	struct markov_entry **tmp = (struct markov_entry **) malloc(sizeof(struct markov_entry *) * count);
	for (ix = 0; ix < count; ix++) {
	    //allocate memory for rows
	    tmp[ix] = (struct markov_entry *) calloc(sizeof(struct markov_entry), count);
	}
	graph->cur_row = 0;
	graph->cur_col = 0;
	graph->count = count;
	graph->entrys = tmp;
	graph->transition_cnt = 0;
	int kx = 0;
	char *pos;
	while (kx < bracnt) {
	    pos = strtok(bra_next, "|");
	    tmp[kx][0].adjacent_node_len = atoi(pos);
	    tmp[kx][0].node_len = tmp[kx][0].adjacent_node_len;
	    pos += strlen(pos) + 1;
	    bra_next = pos + 1;
	    int n = strlen(pos);
	    bra_next += n;
	    char *ket_prob_list = malloc(n + 1);
	    strcpy(ket_prob_list, pos);
	    found = strtok(ket_prob_list, ",");
	    double prevtot = 0;
	    int cx = 0;
	    while (found != NULL) {
		char *end;
		tmp[kx][cx].prob = strtof(found, &end);
		if (*end != '\0') {
		    fprintf (stderr, "Invalid value of '%s'\n", found);
		    markov_graph_free(graph);
		    free(ket_prob_list);
		    graph = NULL;
		    goto ERR_EXIT;
		}
		if (FloatLessThanZero(tmp[kx][cx].prob) || FloatGreaterThanOne(tmp[kx][cx].prob)) {
		    fprintf (stderr, "Probability must be between 0 and 1 but is %f\n", tmp[kx][cx].prob);
		    markov_graph_free(graph);
		    free(ket_prob_list);
		    graph = NULL;
		    goto ERR_EXIT;
		}
		tmp[kx][cx].prob_cummulative = FloatEqualZero(tmp[kx][cx].prob) ? prevtot : (tmp[kx][cx].prob + prevtot);
		if (FloatGreaterThanOne(tmp[kx][cx].prob_cummulative)) {
		    fprintf (stderr, "Cummulative probability for row %d can't be greater than 1 but is %f\n", kx, tmp[kx][cx].prob_cummulative);
		    markov_graph_free(graph);
		    free(ket_prob_list);
		    graph = NULL;
		    goto ERR_EXIT;
		}
		prevtot = tmp[kx][cx].prob_cummulative;
		cx++;
		found = strtok(NULL, ",");
	    }
	    if (cx != bracnt) {
		fprintf (stderr, "malformed: row column expected %dx%d with '%s' row of %d\n", bracnt, bracnt, pos, cx);
	    }
	    if (FloatLessThanOne(tmp[kx][bracnt-1].prob_cummulative)) {
		fprintf (stderr, "Cummulative probability for row %d less than 1 and is %f\n", kx, tmp[kx][bracnt-1].prob_cummulative);
		markov_graph_free(graph);
		free(ket_prob_list);
		graph = NULL;
		goto ERR_EXIT;
		return NULL;
	    }
	    kx++;
	    if (ket_prob_list) {
		free(ket_prob_list);
		ket_prob_list = NULL;
	    }
	}
	for (int cx = 0; cx < bracnt; cx++) {
	    for (int rx = 0; rx < bracnt; rx++) {
		tmp[rx][cx].adjacent_node_len = tmp[cx][0].node_len;
	    }
	}
	graph->node_known = false;
    }
    // markov_graph_print(graph);
  ERR_EXIT:
    if (tmp_bra)
	free(tmp_bra);
    return graph;
}

int markov_graph_next (struct markov_graph *graph) {
    struct markov_entry **tmp;
    tmp = graph->entrys;
    graph->transition_cnt++;
    float pull_rand = (float)rand()/(float)(RAND_MAX);
    int ix = 0;
    while ((ix < graph->count) && (tmp[graph->cur_row][ix++].prob_cummulative < pull_rand)) {}
    while (FloatEqualZero(tmp[graph->cur_row][--ix].prob)) {}
    tmp[graph->cur_row][ix].transition_cnt++;
    graph->cur_row = ix;
    return tmp[graph->cur_row][0].node_len;
}

bool markov_graph_count_edge_transition (struct markov_graph *graph, int node_len) {
    struct markov_entry **tmp;
    tmp = graph->entrys;
    graph->transition_cnt++;
    int ix = 0;
    bool found = false;
    if (!graph->node_known) {
	for (ix = 0; ix < graph->count; ix++) {
	    if (tmp[ix][0].node_len == node_len) {
		graph->node_known = true;
		break;
	    }
	}
	if (graph->node_known) {
	    graph->cur_row = ix;
	} else {
	    graph->unknown_cnt++;
	}
    } else {
	while (!found && (ix < graph->count)) {
	    if (tmp[graph->cur_row][ix].adjacent_node_len == node_len) {
		found = true;
	    } else {
		ix++;
	    }
	}
	if (found) {
	    tmp[graph->cur_row][ix].transition_cnt++;
	    graph->cur_row = ix;
	} else {
	    graph->unknown_cnt++;
	    ix = 0;
	    graph->node_known = false;
	}
    }
    return found;
}

void markov_graph_set_seed (struct markov_graph *graph, int seed) {
    graph->seed = seed;
    srand(seed);
}

#if 0
int main () {
    char braket_option[] = "<256| 0.1,0.7,0.2<1024|0.3,0.4,0.3  <1470|0.4,0.4,0.2";
    struct markov_graph *graph = markov_graph_init(braket_option);
    struct markov_entry **tmp;
    tmp = graph->entrys;
    markov_graph_print(graph);
    int ix;
    int prevlen = 0;
    int len =0;
    for (ix = 0; ix < 1000; ix++) {
	prevlen = len;
	len = markov_graph_next(graph);
	printf("*** len = %d:%d\n", prevlen, len);
    }
    markov_graph_free(graph);
}
#endif
