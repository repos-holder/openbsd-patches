/*     $OpenBSD: smtp_session.c,v 1.149 2011/10/03 19:20:51 gilles Exp $       */

char *evbuffer_readln_crlf(struct evbuffer *);

char *
evbuffer_readln_crlf(struct evbuffer *buffer)
{
        u_char *data = EVBUFFER_DATA(buffer);
        size_t len = EVBUFFER_LENGTH(buffer);
        char *line;
        unsigned int i, j;

        for (i = 0; i < len; ++i) {
                if (data[i] == '\n')
                        break;
        }

        if (i == len)
                return NULL;

        j = i;
        if (i != 0 && data[i - 1] == '\r')
                --j;

        line = calloc(j + 1, 1);
        if (line == NULL)
                fatal("calloc");

        if (j != 0)
                memcpy(line, data, j);

        evbuffer_drain(buffer, i + 1);

        return (line);
}
