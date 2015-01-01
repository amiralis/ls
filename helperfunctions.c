#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
/*
This function returns a string representing the user, group and other permissions of the file represtened by struct stat s.
The string is nine charaters long and terminted with a '\0' character.
*/
char *make_permissions_string(struct stat* s){
	char *perm=malloc(11);
	strcpy(perm,"----------");

	int i=0;
	mode_t mode = s->st_mode;
	if(S_ISDIR(mode))perm[i]='d';
	i++;

	if(S_IRUSR&mode)perm[i]='r';
	i++;
	if(S_IWUSR&mode)perm[i]='w';
	i++;
	if(S_IXUSR&mode)perm[i]='x';
	i++;

	if(S_IRGRP&mode)perm[i]='r';
	i++;
	if(S_IWGRP&mode)perm[i]='w';
	i++;
	if(S_IXGRP&mode)perm[i]='x';
	i++;

	if(S_IROTH&mode)perm[i]='r';
	i++;
	if(S_IWOTH&mode)perm[i]='w';
	i++;
	if(S_IXOTH&mode)perm[i]='x';

	return perm;
}

/*
This function returns a string containing the user and group name (seperated by a single space) of the file represtened by struct stat s.
The string is terminted with a '\0' character.
*/
char *make_user_group_string(struct stat* s){
	struct passwd *pw = getpwuid(s->st_uid);
	if(pw==NULL)exit(EXIT_FAILURE);
	struct group *gr = getgrgid(s->st_gid);
	if(gr==NULL)exit(EXIT_FAILURE);
	//Final string is pw->pw_name) + ' ' + gr->gr_name + '\0'
	int size=strlen(pw->pw_name)+strlen(gr->gr_name)+2;
	char *result= (char*)malloc(size);
	sprintf(result,"%s %s",pw->pw_name,gr->gr_name);
	return result;
}



char *format_time(struct timespec cal_time)
{
  struct tm *time_struct;
  static char string[30];

  time_struct=localtime(&cal_time);

  strftime(string, sizeof string, "%h %e %H:%M", time_struct);

  return(string);
}

