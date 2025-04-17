/*---------------------------------------------------------------
 * Copyright (c) 1999,2000,2001,2002,2003,2023
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
 * histogram.h
 * Suppport for isochonronous traffic testing
 *
 * by Robert J. McMahon (rjmcmahon@rjmcmahon.com, bob.mcmahon@broadcom.com)
 * -------------------------------------------------------------------
 */
#ifndef BPFSC_H
#define BPFSC_H

#ifdef __cplusplus
extern "C" {
#endif

extern int SockBPF_Drop_All (int sock);
extern int SockBPF_v4_Accept (int sock, uint16_t port);
extern int SockBPF_Accept_V4_TAP (int sock, uint32_t dstip, uint16_t port);
extern int SockBPF_v4_Connect (int sock, uint32_t dstip, uint32_t srcip, uint16_t dstport, uint16_t srcport);
extern int SockBPF_v4_Connect_TAP (int sock, uint32_t dstip, uint32_t srcip, uint16_t dstport, uint16_t srcport);
extern int SockBPF_v4_Connect_Drop (int sock, uint32_t dstip, uint32_t srcip, uint16_t dstport, uint16_t srcport);
extern int SockBPF_v6_Connect (int sock, struct in6_addr *dst, struct in6_addr *src, uint16_t dstport, uint16_t srcport);

#ifdef __cplusplus
} /* end extern "C" */
#endif
#endif // BPFSC_H
