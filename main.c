#include <stdlib.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <inttypes.h>

// TODO REMOVE
#include <stdio.h>
#include <string.h>

struct mach_header_64	*read_header(int fd)
{
	struct mach_header_64	*header;
	int offset = 0;

	header = malloc(sizeof(struct mach_header_64));
	while (offset < sizeof(struct mach_header_64))
		offset += read(fd, header + offset, sizeof(struct mach_header_64) - offset);
	if (header->magic != MH_MAGIC_64 && header->magic != MH_CIGAM_64) // TODO handle reverse byte order
	{
		printf("Not an object file.\n");
		return (NULL);
	}
	printf("mach_header_64 read:\n");
	printf("magic = %" PRId32 "\n", header->magic);
	printf("cputype = %" PRId32 "\n", header->cputype);
	printf("cpusubtype = %" PRId32 "\n", header->cpusubtype);
	printf("filetype = %" PRId32 " (MH_OBJECT = %" PRId32 ")\n", header->filetype, MH_OBJECT);
	printf("ncmds = %" PRId32 "\n", header->ncmds);
	printf("sizeofcmds = %" PRId32 "\n", header->sizeofcmds);
	printf("flags = %" PRId32 "\n", header->flags);
	printf("reserved = %" PRId32 "\n", header->reserved);
	return header;
}

#define PTR_ADD(P, O) (void *)(((char *)P) + O)

void	load_sections_64(struct section_64 *sects, uint32_t nsects)
{
	for (uint32_t i = 0; i < nsects; i++)
	{
		struct section_64 *s = sects + i;
		printf("Section %" PRId32 ":\n", i);
		printf("sectname = %.16s\n", s->sectname);
		printf("segname = %.16s\n", s->segname);
		printf("addr = %" PRId64 "\n", s->addr);
		printf("size = %" PRId64 "\n", s->size);
		printf("offset = %" PRId32 "\n", s->offset);
		printf("align = %" PRId32 "\n", s->align);
		printf("reloff = %" PRId32 "\n", s->reloff);
		printf("nreloc = %" PRId32 "\n", s->nreloc);
		printf("flags = %" PRId32 "\n", s->flags);
		printf("reserved1 = %" PRId32 "\n", s->reserved1);
		printf("reserved2 = %" PRId32 "\n", s->reserved2);
	}
}

void	print_version(uint32_t version)
{
	uint16_t x = version >> 16;
	uint8_t y = (version >> 8) & 0xFFFF;
	uint8_t z = version & 0xFFFF;
	printf("%" PRId16 ".%" PRId8 ".%" PRId8 "\n", x, y, z);
}

void	*read_load(int fd)
{
	struct load_command	header;
	void	*load;

	read(fd, &header, sizeof(struct load_command));
	printf("cmd\t%" PRId32 ", %" PRId32 "\n", header.cmd, header.cmdsize);
	load = malloc(header.cmdsize);
	memcpy(load, &header, sizeof(struct load_command));
	read(fd, PTR_ADD(load, sizeof(struct load_command)), header.cmdsize - sizeof(struct load_command));
	if (header.cmd == LC_SEGMENT_64)
	{
		struct segment_command_64 *cmd = load;
		printf("segment_command_64 read:\n");
		printf("cmd = %" PRId32 "\n", cmd->cmd);
		printf("cmdsize = %" PRId32 "\n", cmd->cmdsize);
		printf("segname = %.16s\n", cmd->segname);
		printf("vmaddr = %" PRId64 "\n", cmd->vmaddr);
		printf("vmsize = %" PRId64 "\n", cmd->vmsize);
		printf("fileoff = %" PRId64 "\n", cmd->fileoff);
		printf("filesize = %" PRId64 "\n", cmd->filesize);
		printf("maxprot = %" PRId32 "\n", cmd->maxprot);
		printf("initprot = %" PRId32 "\n", cmd->initprot);
		printf("nsects = %" PRId32 "\n", cmd->nsects);
		printf("flags = %" PRId32 "\n", cmd->flags);
		load_sections_64(load + sizeof(struct segment_command_64), cmd->nsects);
	}
	else if (header.cmd == LC_SYMTAB)
	{
		struct symtab_command *cmd = load;
		printf("symtab_command read:\n");
		printf("cmd = %" PRId32 "\n", cmd->cmd);
		printf("cmdsize = %" PRId32 "\n", cmd->cmdsize);
		printf("symoff = %" PRId32 "\n", cmd->symoff);
		printf("nsyms = %" PRId32 "\n", cmd->nsyms);
		printf("stroff = %" PRId32 "\n", cmd->stroff);
		printf("strsize = %" PRId32 "\n", cmd->strsize);
	}
	else if (header.cmd == LC_DYSYMTAB)
	{
		struct dysymtab_command *cmd = load;
		printf("dysymtab_command read:\n");
		printf("cmd = %" PRId32 "\n", cmd->cmd);
		printf("cmdsize = %" PRId32 "\n", cmd->cmdsize);
		printf("ilocalsym = %" PRId32 "\n", cmd->ilocalsym);
		printf("nlocalsym = %" PRId32 "\n", cmd->nlocalsym);
		printf("iextdefsym = %" PRId32 "\n", cmd->iextdefsym);
		printf("nextdefsym = %" PRId32 "\n", cmd->nextdefsym);
		printf("iundefsym = %" PRId32 "\n", cmd->iundefsym);
		printf("nundefsym = %" PRId32 "\n", cmd->nundefsym);
		printf("tocoff = %" PRId32 "\n", cmd->tocoff);
		printf("ntoc = %" PRId32 "\n", cmd->ntoc);
		printf("modtaboff = %" PRId32 "\n", cmd->modtaboff);
		printf("nmodtab = %" PRId32 "\n", cmd->nmodtab);
		printf("extrefsymoff = %" PRId32 "\n", cmd->extrefsymoff);
		printf("nextrefsyms = %" PRId32 "\n", cmd->nextrefsyms);
		printf("indirectsymoff = %" PRId32 "\n", cmd->indirectsymoff);
		printf("nindirectsyms = %" PRId32 "\n", cmd->nindirectsyms);
		printf("extreloff = %" PRId32 "\n", cmd->extreloff);
		printf("nextrel = %" PRId32 "\n", cmd->nextrel);
		printf("locreloff = %" PRId32 "\n", cmd->locreloff);
		printf("nlocrel = %" PRId32 "\n", cmd->nlocrel);
	}
	else if (header.cmd == LC_VERSION_MIN_MACOSX
			|| header.cmd == LC_VERSION_MIN_IPHONEOS
			|| header.cmd == LC_VERSION_MIN_WATCHOS
			|| header.cmd == LC_VERSION_MIN_TVOS)
	{
		struct version_min_command *cmd = load;
		printf("version_min_command read:\n");
		printf("cmd = %" PRId32 "\n", cmd->cmd);
		printf("cmdsize = %" PRId32 "\n", cmd->cmdsize);
		printf("version = "); print_version(cmd->version);
		printf("sdk = "); print_version(cmd->sdk);
	}
	else
		printf("unknown\n");
	return (load);
}

int main()
{
	int fd = open("ft_putstr.o", O_RDONLY);
	struct mach_header_64 *header = read_header(fd);
	struct load_command *loads[32];
	for (int i = 0; i < header->ncmds; i++)
		loads[i] = read_load(fd);
}
