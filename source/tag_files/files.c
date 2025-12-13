/*
FILES.C
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "files.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void file_location_set_volume(
	short location,
	const char *volume_name)
{
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 75, location>0 && location<NUMBER_OF_FILE_REFERENCE_LOCATIONS);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 76, strlen(file_location_volume_names[location])==0);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 77, strlen(volume_name)<=MAXIMUM_FILENAME_LENGTH);

	strncpy(file_location_volume_names[location], volume_name, NUMBEROF(*file_location_volume_names)-1);
	file_location_volume_names[location][NUMBEROF(*file_location_volume_names)-1]= '\0';

	return;
}

struct file_reference *file_reference_create(
	struct file_reference *reference,
	short location)
{
	struct file_reference_info *info = (struct file_reference_info *)reference;
    match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 91, info);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 92, location>=NONE && location<NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	memset(info, 0, sizeof(*info));
	info->location= location;
	info->signature= FILE_REFERENCE_SIGNATURE;

	return reference;
}

long find_files(
	unsigned long flags,
	const struct file_reference *directory,
	long maximum_count,
	struct file_reference *references)
{
	long file_count= 0;

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 257, maximum_count>0);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 258, references);

	find_files_start(flags, directory);

	for (file_count; file_count<maximum_count; file_count++, references++)
	{
		if (!find_files_next(references, NULL))
		{
			break;
		}
	}

	return file_count;
}

void *file_read_into_memory(
	struct file_reference *reference,
	unsigned long *size)
{
	void *buffer= NULL;

	if (file_open(reference, FLAG(_permission_read_bit)))
	{
		unsigned long eof= file_get_eof(reference);
		*size= eof;
		buffer= match_malloc(eof, "c:\\halo\\SOURCE\\tag_files\\files.c", 280);

		if (buffer)
		{
			if (!file_read(reference, *size, buffer))
			{
				match_free(buffer, "c:\\halo\\SOURCE\\tag_files\\files.c", 286);
				buffer= NULL;
			}
		}

		file_close(reference);
	}

	return buffer;
}

/* ---------- private code */
