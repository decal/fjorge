#include"fjorge.h"

long fjcb_bio_debug(BIO * bio, int cmd, const char *argp,
    int argi, long argl, long ret)
{
        BIO *out;

        out = (BIO *) BIO_get_callback_arg(bio);
        if (out == NULL)
                return (ret);

        if (cmd == (BIO_CB_READ | BIO_CB_RETURN)) {
                BIO_printf(out,
                    "read from %p [%p] (%lu bytes => %ld (0x%lX))\n",
                    (void *) bio, argp, (unsigned long) argi, ret, ret);
                BIO_dump(out, argp, (int) ret);
                return (ret);
        } else if (cmd == (BIO_CB_WRITE | BIO_CB_RETURN)) {
                BIO_printf(out,
                    "write to %p [%p] (%lu bytes => %ld (0x%lX))\n",
                    (void *) bio, argp, (unsigned long) argi, ret, ret);
                BIO_dump(out, argp, (int) ret);
        }
        return (ret);
}
