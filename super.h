#include <linux/init.h>
#include <linux/module.h>


static void sfs_put_super(struct super_block *sb);

static struct super_operations const sfs_super_ops = {
	.put_super = sfs_put_super,
};


static int sfs_fill_sb(
		struct super_block *sb,
		void* data, 
		int silent);

static struct dentry *sfs_mount(
		struct file_system_type *type,
		int flags,
		char const *dev,
		void * data);


static struct file_system_type sfs_type = {
	.owner = THIS_MODULE,
	.name = "sfs",
	.mount = sfs_mount,
	.kill_sb = kill_block_super,
	.fs_flags = FS_REQUIRES_DEV
};

