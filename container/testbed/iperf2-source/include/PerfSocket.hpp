/*---------------------------------------------------------------
 * Copyright (c) 1999,2000,2001,2002,2003
 * The Board of Trustees of the University of Illinois
 * All Rights Reserved.
 *---------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software (Iperf) and associated
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
 * Neither the names of the University of Illinois, NCSA,
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
 * National Laboratory for Applied Network Research
 * National Center for Supercomputing Applications
 * University of Illinois at Urbana-Champaign
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________
 *
 * PerfSocket.hpp
 * by Mark Gates <mgates@nlanr.net>
 *  &  Ajay Tirumala <tirumala@ncsa.uiuc.edu>
 * -------------------------------------------------------------------
 * Changes in version 1.6
 *   Incorporates class declarations for fetching data from files
 * ------------------------------------------------------------------- */


#ifndef PERFSOCKET_H
#define PERFSOCKET_H

#include "Mutex.h"
#include "Settings.hpp"
#ifdef __cplusplus
extern "C" {
#endif

// int timer units is micorseconds
void SetSocketOptions(struct thread_Settings *inSettings);
void SetSocketOptionsSendTimeout(struct thread_Settings *mSettings, int timer);
void SetSocketOptionsReceiveTimeout(struct thread_Settings *mSettings, int timer);
void SetSocketOptionsIPTos (struct thread_Settings *mSettings, int tos);
void SetSocketTcpTxDelay (struct thread_Settings *mSettings, int delay);
void SetSocketBindToDeviceIfNeeded (struct thread_Settings *inSettings);
void SetSocketOptionsIPRCVTos (struct thread_Settings *mSettings);
void setsock_tcp_mss(int inSock, int inMSS);
int  getsock_tcp_mss(int inSock);
#ifdef DEFAULT_PAYLOAD_LEN_PER_MTU_DISCOVERY
void checksock_max_udp_payload (struct thread_Settings *inSettings);
#endif

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif // PERFSOCKET_H
