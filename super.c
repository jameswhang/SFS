#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "super.h"

#define SFS_MAGIC_NUMBER	0xdeadbeef

static int __init sfs_init(void) {
	pr_debug("sfs module loaded\n");
	register_filesystem(&sfs_type);
	printk("SFS module loaded\n");
	return 0;
}

static void __exit sfs_finish(void) {
	pr_debug("sfs module unloaded\n");
	printk("SFS module unloaded\n");
}

static int sfs_fill_sb(
		struct super_block *sb,
		void* data, 
		int silent) 
{
	struct inode *root = NULL;
	sb->s_magic = SFS_MAGIC_NUMBER;
	sb->s_op = &sfs_super_ops;

	root = new_inode(sb);

	if (!root) {
		printk("inode allocation failed\n");
		return -ENOMEM;
	}

	root->i_ino = 0;
	root->i_sb = sb;
	root->i_atime = root->i_mtime = root->i_ctime = CURRENT_TIME;
	inode_init_owner(root, NULL, S_IFDIR);

	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		printk("Root creation failed\n");
		return -ENOMEM;
	}
	return 0;
}

static struct dentry *sfs_mount(
		struct file_system_type *type,
		int flags,
		char const *dev,
		void * data)
{
	struct dentry *const entry = mount_bdev(type, flags, dev, data, sfs_fill_sb);
	if (IS_ERR(entry)) {
		printk("sfs mounting failed\n");
	} else {
		printk("SFS Mounted!\n");
	}
	return entry;
}

static void sfs_put_super(struct super_block *sb) {
	printk("SFS super block destroyed\n");
}

module_init(sfs_init);
module_exit(sfs_finish);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("James Whang");
