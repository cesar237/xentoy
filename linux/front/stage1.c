#include <linux/module.h>
#include <xen/xen.h>
#include <xen/xenbus.h>

static void cmd_changed(struct xenbus_watch *watch, const char *path, 
    const char *token)
{
    /* Handle the command change here */
    char *val = xenbus_read(XBT_NIL, path, "", NULL);
    if (IS_ERR(val)) {
        pr_err("xentoy: failed to read %s\n", path);
        return;
    }
    /* Process the value here */
    pr_info("xentoy: %s = %s\n", path, val);
    kfree(val);
}

static struct xenbus_watch cmd_watch = {
    .node = "data/xentoy-cmd",
    .callback = cmd_changed,
};

static int __init xentoy_init(void) {
    struct xenbus_transaction xbt;
    unsigned int domid;
    int err;

    if (!xen_domain()) {
        pr_err("xentoy: not running in a Xen domain\n");
        return -ENODEV;
    }

    if (xenbus_scanf(XBT_NIL, "domid", "%u", &domid) == 1)
        pr_info("xentoy: domid %u\n", domid);
    else
        pr_err("xentoy: failed to read domid from xenstore\n");

again:
    err = xenbus_transaction_start(&xbt);
    if (err) {
        pr_err("xentoy: failed to start xenbus transaction\n");
        return err;
    }
    xenbus_printf(xbt, "data", "xentoy-a", "%d", 1);
    xenbus_printf(xbt, "data", "xentoy-b", "%d", 2);
    
    err = xenbus_transaction_end(xbt, 0);
    if (err) {
        pr_err("xentoy: failed to end xenbus transaction\n");
        goto again;
    }

    return register_xenbus_watch(&cmd_watch);
}

static void __exit xentoy_exit(void) {
    unregister_xenbus_watch(&cmd_watch);
}

module_init(xentoy_init);
module_exit(xentoy_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Xentoy Xenbus Frontend");
MODULE_AUTHOR("Cesaire M.H.");