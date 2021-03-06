/*
 * Copyright (C) 2014 DarkSide Project
 * Authored by George Klees <gksharkboy@gmail.com>
 * access.c - Security descriptor access checking
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public Licens
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <types.h>
#include <iterator.h>
#include <list.h>
#include <map.h>
#include <security/uid.h>
#include <security/token.h>
#include <security/acl.h>
#include <security/sd.h>
#include <security/access.h>

/* See if an ACE refers to a token */
static bool ace_matches_token(ace_t *entry, token_t *token)
{
	/* ACE mentions our user or group */
	bool mentions_user = false, mentions_group = false;

	/* Check if it mentions our user or group */
	if (entry->type == ACE_USER && entry->uid == token->user)
	{
		mentions_user = true;
	}
	if (entry->type == ACE_GROUP && (entry->gid == token->primary_group || map_get(&token->other_groups, entry->gid)))
	{
		mentions_group = true;
	}

	return mentions_user || mentions_group;
}

/* Check an access token against a security descriptor */
bool sd_check_access(security_descriptor_t *descriptor, token_t *token, access_mask_t desired_access)
{
	/* Current level of access granted */
	uint64_t granted_access = 0;

	/* First make sure none of our desired permissions are denied */
	iterator_t iter = list_head(&descriptor->denied_object_acl->entries);

	ace_t *entry = (ace_t*) iter_now(&iter);
	while (entry)
	{
		/* Skip the ACE if it doesn't mention our user or group */
		if (!ace_matches_token(entry, token))
		{
			entry = (ace_t*) iter_next(&iter);
		}

		/* If one of our desired access rights is denied, fail the check */
		if (desired_access & entry->access_mask)
		{
			return false;
		}

		/* Go to the next ACE */
		entry = (ace_t*) iter_next(&iter);
	}

	/* Go through all the allowed permissions and record the ones we get */
	iter = list_head(&descriptor->allowed_object_acl->entries);

	entry = (ace_t*) iter_now(&iter);
	while (entry)
	{
		/* Skip the ACE if it doesn't mention our user or group */
		if (!ace_matches_token(entry, token))
		{
			entry = (ace_t*) iter_next(&iter);
		}

		/* Record the rights it gives us */
		granted_access |= entry->access_mask;

		/* If we got all needed permissions, return true */
		if ((granted_access & desired_access) == desired_access)
		{
			return true;
		}

		/* Go to the next ACE */
		entry = (ace_t*) iter_next(&iter);
	}

	/* If we didn't get access by now, we lack some permission */
	return false;
}
