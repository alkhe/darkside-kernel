#include <types.h>
#include <string.h>
#include <stdlib.h>
#include <iterator.h>
#include <list.h>
#include <dict.h>
#include <fs/fs.h>

/* Filesystems and mountpoints lists */
static dict_t filesystems;
static list_t mountpoints;

/* Root of the filesystem */
static inode_t *root;

/* Register a filesystem */
int fs_register(char *fs_name, filesystem_ops_t *ops)
{
	return dict_append(&filesystems, fs_name, ops);
}

/* Unregister a filesystem */
int fs_unregister(char *fs_name)
{
	dict_remove(&filesystems, fs_name);
	return 0;
}

/* Mount a filesystem */
int fs_mount(device_t *device, char *path, char *fs_name)
{
	/* Open the node to mount */
	inode_t *node = fs_open(path);

	/* Is the filesystem already mounted? */
	iterator_t iter = list_head(&mountpoints);

	mountpoint_t *mp = (mountpoint_t*) iter.now(&iter);
	while (mp)
	{
		/* Does the node match */
		if (mp->node == node)
		{
			return -1;
		}

		mp = (mountpoint_t*) iter.next(&iter);
	}

	/* Get the filesystem operations */
	filesystem_ops_t *ops = dict_get(&filesystems, fs_name);

	/* Is the filesystem registered? */
	if (ops)
	{
		/* Initialize the filesystem */
		filesystem_t *filesystem = (filesystem_t*) malloc(sizeof(filesystem_t));
		ops->init(filesystem, device);

		/* Create and set up the mountpoint */
		mp = (mountpoint_t*) malloc(sizeof(mountpoint_t));
		mp->node = node;
		mp->filesystem = filesystem;
		memcpy(&mp->orig_node_data, node, sizeof(inode_t));

		/* Replace the current inode with the filesystem root */
		memcpy(node, &filesystem->root, sizeof(inode_t));

		/* Add the mountpoint to the list */
		list_insert_tail(&mountpoints, &mp);

		return 0;
	}

	return -1;
}

/* Unmount a filesystem */
int fs_unmount(char *path)
{
	/* Open the node to mount */
	inode_t *node = fs_open(path);

	/* Is the filesystem mounted? */
	iterator_t iter = list_head(&mountpoints);

	mountpoint_t *mp = (mountpoint_t*) iter.now(&iter);
	while (mp)
	{
		/* Does the node match */
		if (mp->node == node)
		{
			/* Restore the inode's original data */
			memcpy(node, &mp->orig_node_data, sizeof(inode_t));

			/* Remove the mountpoint from the list */
			iter.remove(&iter);

			/* Free the mountpoint's memory */
			free(mp);

			return 0;
		}

		mp = (mountpoint_t*) iter.next(&iter);
	}

	return -1;
}

/* Open an inode */
inode_t *fs_open(char *path)
{
	/* Set the current inode to the filesystem root */
	inode_t *current = root;

	/* Go through each element of the path */
	char *save, *element;
	element = strtok_r(path, "/", &save);
	while (element)
	{
		current = current->ops->finddir(&current, element);

		if (!current)
		{
			return NULL;
		}

		element = strtok_r(NULL, "/", &save);
	}

	return current;
}

/* Read data from an inode into a buffer */
uint64_t fs_read(inode_t *node, void *buffer, uint64_t offset, uint64_t length)
{
	if (node->ops->read)
	{
		return node->ops->read(node, buffer, offset, length);
	}

	return 0;
}

/* Write data from a buffer into an inode */
uint64_t fs_write(inode_t *node, void *buffer, uint64_t offset, uint64_t length)
{
	if (node->ops->write)
	{
		return node->ops->write(node, buffer, offset, length);
	}

	return 0;
}