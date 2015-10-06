/*	$gate: event.h,v 1.1 2010/12/14 02:43:05 dukzcry Exp $
/*	$OpenBSD: event.c,v 1.25 2010/08/30 07:54:29 nicm Exp $	*/

/*
 * Copyright (c) 2000-2004 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/queue.h>
#include <sys/tree.h>

#include <signal.h>
#include <event.h>

/* Structures */

struct evsignal_info {
        struct event_list signalqueue;
        struct event ev_signal;
        int ev_signal_pair[2];
        int ev_signal_added;
        volatile sig_atomic_t evsignal_caught;
        sig_atomic_t evsigcaught[NSIG];
};

struct event_base {
        const struct eventop *evsel;
        void *evbase;
        int event_count;                /* counts number of total events */
        int event_count_active; /* counts number of active events */

        int event_gotterm;              /* Set to terminate loop */

        /* active event management */
        struct event_list **activequeues;
        int nactivequeues;

        /* signal handling info */
        struct evsignal_info sig;

        struct event_list eventqueue;
        struct timeval event_tv;

        RB_HEAD(event_tree, event) timetree;
};

/* Functions */

void
event_queue_remove(struct event_base *base, struct event *ev, int queue)
{
        if (!(ev->ev_flags & queue))
                event_errx(1, "%s: %p(fd %d) not on queue %x", __func__,
                           ev, ev->ev_fd, queue);

        if (~ev->ev_flags & EVLIST_INTERNAL)
                base->event_count--;

        ev->ev_flags &= ~queue;
        switch (queue) {
        case EVLIST_ACTIVE:
                base->event_count_active--;
                TAILQ_REMOVE(base->activequeues[ev->ev_pri],
                    ev, ev_active_next);
                break;
        case EVLIST_SIGNAL:
                TAILQ_REMOVE(&base->sig.signalqueue, ev, ev_signal_next);
                break;
        case EVLIST_TIMEOUT:
                RB_REMOVE(event_tree, &base->timetree, ev);
                break;
        case EVLIST_INSERTED:
                TAILQ_REMOVE(&base->eventqueue, ev, ev_next);
                break;
        default:
                event_errx(1, "%s: unknown queue %x", __func__, queue);
        }
}

/* reinitialized the event base after a fork */
int
event_reinit(struct event_base *base)
{
        const struct eventop *evsel = base->evsel;
        void *evbase = base->evbase;
        int res = 0;
        struct event *ev;

#if 0
        /* Right now, reinit always takes effect, since even if the
           backend doesn't require it, the signal socketpair code does.
        */
        /* check if this event mechanism requires reinit */
        if (!evsel->need_reinit)
                return (0);
#endif

        /* prevent internal delete */
        if (base->sig.ev_signal_added) {
                /* we cannot call event_del here because the base has
                 * not been reinitialized yet. */
                event_queue_remove(base, &base->sig.ev_signal,
                    EVLIST_INSERTED);
                if (base->sig.ev_signal.ev_flags & EVLIST_ACTIVE)
                        event_queue_remove(base, &base->sig.ev_signal,
                            EVLIST_ACTIVE);
                base->sig.ev_signal_added = 0;
        }

        if (base->evsel->dealloc != NULL)
                base->evsel->dealloc(base, base->evbase);
        evbase = base->evbase = evsel->init(base);
        if (base->evbase == NULL)
                event_errx(1, "%s: could not reinitialize event mechanism",
                    __func__);

        TAILQ_FOREACH(ev, &base->eventqueue, ev_next) {
                if (evsel->add(evbase, ev) == -1)
                        res = -1;
        }

        return (res);
}
