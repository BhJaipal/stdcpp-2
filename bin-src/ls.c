#include <fs.h>
#include <unistd.h>
#include <c-impl.h>
#include <malloc.h>
#include <string.h>
#include <io.h>
#include <file.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

enum {
	HIDDEN_ENABLE= 1,
	FULL_DETAIL = 2
};

void print_permission(int permissions) {
	switch (permissions) {
		case 0:
			printf("---");
			break;
		case 1:
			printf("--x");
			break;
		case 2:
			printf("-w-");
			break;
		case 3:
			printf("-wx");
			break;
		case 4:
			printf("r--");
			break;
		case 5:
			printf("r-x");
			break;
		case 6:
			printf("rw-");
			break;
		case 7:
			printf("rwx");
			break;
	}
}

int main(int argc, char **argv) {
	int                  fd;
	char                 buf[BUF_SIZE];
	long                 nread;
	linux_dirent64 *d;
	size_t total_size = 0;

	char dir[100] = ".";
	char flags;
	if (argc != 1) {
		if (argv[1][0] == '-') {
			int i = 1;
			while (argv[1][i]) {
				if (argv[1][i] == 'l')
					flags |= FULL_DETAIL;
				else if (argv[1][i] == 'a')
					flags |= HIDDEN_ENABLE;
				i++;
			}
			if (argc > 2) {
				strcpy(dir, argv[2]);
			}
		} else {
			strcpy(dir, argv[2]);
		}
	}
	fd = open(dir, O_RDONLY | O_DIRECTORY);
	if (fd == -1) {
		dprintf(2, "directory does not exist\n");
		return 1;
	}
	int i = 0;
	for (;;) {
		nread = getdents64(fd, (linux_dirent64*)buf, BUF_SIZE);
		if (nread == -1)
			break;

		if (nread == 0)
			break;

		for (size_t bpos = 0; bpos < nread;) {
			d = (linux_dirent64*)(buf + bpos);
			if (d->d_name[0] == '.' && !(flags & HIDDEN_ENABLE))
				goto next;

			Stat statb;
			if (d->d_type == DT_LNK) {
				if (!strcmp(dir, "."))
					lstat(d->d_name, &statb);
				else {
					char full_path[100] = "";
					strcpy(full_path, dir);
					strcat(full_path, "/");
					strcat(full_path, d->d_name);
					lstat(full_path, &statb);
				}
			} else {
				if (!strcmp(dir, "."))
					stat(d->d_name, &statb);
				else {
					char full_path[100] = "";
					strcpy(full_path, dir);
					strcat(full_path, "/");
					strcat(full_path, d->d_name);
					stat(full_path, &statb);
				}
			}
			int permissions = statb.st_mode;
			permissions &= 0777;

			int root = permissions >> 6;
			int group = (permissions >> 3) & 07;
			int other = permissions & 07;
			if (flags & FULL_DETAIL) {
				switch (d->d_type) {
					case DT_DIR:
						printf("d");
						break;
					case DT_CHR:
						printf("c");
						break;
					case DT_LNK:
						printf("s");
						break;
					default:
						printf("-");
						break;
				}

				print_permission(root);
				print_permission(group);
				print_permission(other);
				if (statb.st_nlink / 100)
					printf(" %lu ", statb.st_nlink);
				else if (statb.st_nlink / 10)
					printf("  %lu ", statb.st_nlink);
				else
					printf("   %lu ", statb.st_nlink);
				int passwd = open("/etc/passwd", O_RDONLY);

				char uid[22];
				char gid[22];
				char buf[100];
				memset(uid, 0, 22);
				memset(gid, 0, 22);
				int i = 0;
				size_t pos = 0;
				while (true) {
					i = 0;
					memset(buf, 0, 100);
					lseek(fd, pos, SEEK_CUR);
					while (true) {
						read(passwd, buf + i, 1);
						if (i == 0 && buf[i] == '\n') { goto end; }
						if (buf[i] == '\n') break;
						i++;
					}
					pos += i;
					buf[i] = 0;
					int found = 0;
					size_t id_name_end = 0;
					for (int f = 0; f <= i; f++) {
						if (buf[f] == ':') {
							if (!found) {
								id_name_end = f;
								found++;
								continue;
							}
							found++;
							char num[10] = "";
							for (size_t v = 0; buf[v+1+f] != ':'; v++) {
								num[v] = buf[f+v+1];
							}
							if (found == 2 && atoi(num) == statb.st_uid) {
								memcpy(uid, buf, id_name_end);
								uid[f] = 0;
							} else if (found == 3 && atoi(num) == statb.st_gid) {
								memcpy(gid, buf, id_name_end);
								gid[f] = 0;
							} else if (found == 3) {
								break;
							}
							if (*uid && *gid) goto end;
						}
					}
				}
			end:
				printf("%s %s ", uid, gid);

				if (statb.st_size / 1000000000) {
					printf("  %liG", (statb.st_size / 1000000000));
				} else if (statb.st_size / 100000000) {
					printf("%liM", (statb.st_size / 1000000));
				} else if (statb.st_size / 10000000) {
					printf(" %liM", (statb.st_size / 1000000));
				} else if (statb.st_size / 1000000) {
					printf("  %liM", (statb.st_size / 1000000));
				} else if (statb.st_size / 100000) {
					printf("%liK", (statb.st_size / 1000));
				} else if (statb.st_size / 10000) {
					printf(" %liK", (statb.st_size / 1000));
				} else if (statb.st_size / 1000) {
					printf("  %liK", (statb.st_size / 1000));
				} else if (statb.st_size / 100) {
					printf(" %li", (statb.st_size));
				} else if (statb.st_size / 10) {
					printf("  %li", (statb.st_size));
				} else {
					printf("   %li", statb.st_size);
				}
				printf(" ");
				total_size += statb.st_size;
			}
			switch (d->d_type) {
				case DT_DIR:
					printf("\e[1;94m");
					break;
				case DT_CHR:
					printf("\e[1;93m");
					break;
				case DT_LNK:
					printf("\e[1;95m");
					break;
			}
			if (d->d_type != DT_DIR && d->d_type != DT_LNK && other & 1) printf("\e[1;92m");
			printf("%s", d->d_name);
			write(1, "\e[0m", 4);
			if (!(flags & FULL_DETAIL)) {
				for (int i = strlen(d->d_name); i <= 20; i++) {
					write(1, " ", 1);
				}
				if (strlen(d->d_name) >= 20) write(1, "  ", 2);
				if (!((i+1) % 10)) printf("\n");
				i++;
			}
			if (flags & FULL_DETAIL) printf("\n");
next:
			bpos += d->d_reclen;
		}
	}
	if (i % 10) printf("\n");
	if (flags & FULL_DETAIL) {
		printf("Total: ");
		if (total_size / 1000000000) {
			printf("%liG", (total_size / 1000000000));
		} else if (total_size / 1000000) {
			printf("%liM", (total_size / 1000000));
		} else if (total_size / 1000) {
			printf("%liK", (total_size / 1000));
		} else {
			printf("%li", total_size);
		}
		printf("\n");
	}
}
