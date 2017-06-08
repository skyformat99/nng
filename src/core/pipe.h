//
// Copyright 2017 Garrett D'Amore <garrett@damore.org>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef CORE_PIPE_H
#define CORE_PIPE_H

// NB: This structure is supplied here for use by the CORE. Use of this
// OUSIDE of the core is STRICTLY VERBOTEN.  NO DIRECT ACCESS BY PROTOCOLS OR
// TRANSPORTS.

#include "core/defs.h"
#include "core/thread.h"
#include "core/transport.h"

struct nni_pipe {
	uint32_t	p_id;
	nni_tran_pipe	p_tran_ops;
	void *		p_tran_data;
	void *		p_proto_data;
	nni_list_node	p_sock_node;
	nni_list_node	p_ep_node;
	nni_sock *	p_sock;
	nni_ep *	p_ep;
	int		p_reap;
	nni_mtx		p_mtx;
	int		p_refcnt;
};

extern int nni_pipe_sys_init(void);
extern void nni_pipe_sys_fini(void);

// AIO
extern int nni_pipe_aio_recv(nni_pipe *, nni_aio *);
extern int nni_pipe_aio_send(nni_pipe *, nni_aio *);

// Pipe operations that protocols use.
extern uint32_t nni_pipe_id(nni_pipe *);
extern void nni_pipe_close(nni_pipe *);
extern void nni_pipe_incref(nni_pipe *);
extern void nni_pipe_decref(nni_pipe *);

// Used only by the socket core - as we don't wish to expose the details
// of the pipe structure outside of pipe.c.
extern int nni_pipe_create(nni_pipe **, nni_ep *, nni_sock *, nni_tran *);

extern void nni_pipe_destroy(nni_pipe *);

extern uint16_t nni_pipe_proto(nni_pipe *);
extern uint16_t nni_pipe_peer(nni_pipe *);
extern int nni_pipe_start(nni_pipe *);
extern int nni_pipe_getopt(nni_pipe *, int, void *, size_t *sizep);

// nni_pipe_set_proto_data sets the protocol private data.  No locking is
// performed, and this routine should only be called once per pipe at
// initialization.
extern void nni_pipe_set_proto_data(nni_pipe *, void *);

// nni_pipe_get_proto_data gets the protocol private data set with the
// nni_pipe_set_proto_data function.  No locking is performed.
extern void *nni_pipe_get_proto_data(nni_pipe *);

// nni_pipe_sock_list_init initializes a list of pipes, to be used by
// a per-socket list.
extern void nni_pipe_sock_list_init(nni_list *);

// nni_pipe_ep_list_init initializes a list of pipes, to be used by
// a per-endpoint list.
extern void nni_pipe_ep_list_init(nni_list *);

#endif // CORE_PIPE_H
