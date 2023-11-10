#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#define max 40

struct file{
    int fperms;
    char fname[max];
    time_t ftimestamp;
    FILE *file;
};

struct flder{
    int perms;
    char name[max];
    time_t timestamp;
    union{
        struct file* subfile;
        struct flder* subflder;
    }*subf;
};

struct file *createfile(struct flder* nflder,int index){
    nflder->subf[index].subfile=(struct file*)malloc(sizeof(struct file));
    struct file* ftmp=nflder->subf[index].subfile;
    scanf("Enter perms 'rwx':%d \n",&ftmp->fperms);
    scanf("Enter name: %s\n",&ftmp->fname);
    ftmp->ftimestamp=time(NULL);
    ftmp->file=NULL;
    return ftmp;
}

struct flder* createflderinflder(struct flder* nflder,int i){
    nflder->subf[i].subflder=(struct flder*)malloc(sizeof(struct flder));
    struct flder* fltmp=nflder->subf[i].subflder;
    scanf("Enter perms 'rwx':%d \n",&fltmp->perms);
    scanf("Enter name: %s\n",&fltmp->name);
    fltmp->timestamp=time(NULL);
    return fltmp;
}

int fcount(struct flder* nflder){
    int i=0;
    while(nflder->subf[i].subfile != NULL){
        i++;
    }
    int j=0;
    while(nflder->subf[j].subflder != NULL){
        j++;
    }
    return i+j;
}

void printflderinfo(struct flder* nflder){
    char* time_str = ctime(&nflder->timestamp);
    printf("Folder Name-->%s\n",nflder->name);
    printf("Folder Size-->%zu\n",sizeof(struct flder));
    printf("Folder Time of Creation-->%s\n",time_str);
    printf("Number of subfiles-->%d\n",fcount(nflder));
    int perms=nflder->perms;
    switch (perms) {
        case 0:
            printf("Folder Permissions : ---\n");
            break;
        case 1:
            printf("Folder Permissions : --x\n");
            break;
        case 2:
            printf("Folder Permissions : -w-\n");
            break;
        case 3:
            printf("Folder Permissions : -wx\n");
            break;
        case 4:
            printf("Folder Permissions : r--\n");
            break;
        case 5:
            printf("Folder Permissions : r-x\n");
            break;
        case 6:
            printf("Folder Permissions : rw-\n");
            break;
        case 7:
            printf("Folder Permissions : rwx\n");
            break;
        default:
            printf("Folder does not exist");
    }
}

void printfileinfo(struct file* nfile){
    char* time_str = ctime(&nfile->ftimestamp);
    printf("File Name-->%s\n",nfile->fname);
    printf("File Size-->%zu\n",sizeof(struct file));
    printf("File Time of creation-->%s\n",time_str);
    int perms=nfile->fperms;
    switch (perms) {
        case 0:
            printf("File Permissions : ---\n");
            break;
        case 1:
            printf("File Permissions : --x\n");
            break;
        case 2:
            printf("File Permissions : -w-\n");
            break;
        case 3:
            printf("File Permissions : -wx\n");
            break;
        case 4:
            printf("File Permissions : r--\n");
            break;
        case 5:
            printf("File Permissions : r-x\n");
            break;
        case 6:
            printf("File Permissions : rw-\n");
            break;
        case 7:
            printf("File Permissions : rwx\n");
            break;
        default:
            printf("File does not exist");
    }
}

void printhierarchy(struct flder* folder, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("Folder: %s\n", folder->name);
    for (int i = 0; folder->subf[i].subflder != NULL; i++) {
        printhierarchy(folder->subf[i].subflder, level + 1);
    for (int i = 0; folder->subf[i].subfile != NULL; i++) {
        for (int j = 0; j < level + 1; j++) {
            printf("  ");
        }
        printf("File: %s\n", folder->subf[i].subfile->fname);
    }
}
}

void execpy(const char* ifile) {
    char command[256];
    snprintf(command, sizeof(command), "python %s", ifile);
    int result = system(command);
    if (result == 0) {
        printf("success\n");
    } else {
        printf("error\n");
    }
}

void executec(const char* ifile) {
    char tmpname[100];
    time_t tstmp = time(NULL);
    snprintf(tmpname, sizeof(tmpname), "temp_%ld", tstmp);
    char compc[100];
    char execc[100];
    snprintf(compc, sizeof(compc), "gcc -o %s %s", tmpname, ifile);
    snprintf(execc, sizeof(execc), "./%s", tmpname);
    if (system(compc) == 0) {
        system(execc);
    } else {
        printf("Compilation failed.\n");
    }
}

void executebash(const char* ifile) {
    char command[100];
    snprintf(command, sizeof(command), "bash %s", ifile);
    if (system(command) == 0) {
    } else {
        printf("failed.\n");
    }
}

void writetofile(struct flder* nflder,int index){
    nflder->subf[index].subfile=(struct file*)malloc(sizeof(struct file));
    struct file* ftmp=nflder->subf[index].subfile;
    ftmp->file = fopen(ftmp->fname, "a+");
    printf("press 'ESC' to stop \n");
    int ch;
    while (1) {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 27) {
                break;
            } else {
                fputc(ch, ftmp->file);
                putchar(ch);
            }
        }
    }
    fclose(ftmp->file);
}

void deleteFile(struct flder *nflder, int index) {
    if (nflder->subf[index].subfile != NULL) {
        free(nflder->subf[index].subfile);
        nflder->subf[index].subfile = NULL;
    } else {
        printf("error.\n");
    }
}

void deleteFolder(struct flder *lfldr, int index) {
    if (lfldr->subf[index].subflder != NULL) {
        free(lfldr->subf[index].subflder);
        lfldr->subf[index].subflder = NULL;
    } else {
        printf("error.\n");
    }
}

void printcontent(struct flder* nflder,int index){
    struct file* ftmp=nflder->subf[index].subfile;
    ftmp->file = fopen(ftmp->fname, "r");
    if (ftmp->file == NULL) {
        perror("File does not exist");
        return;
    }
    char ch;
    while ((ch = fgetc(ftmp->file)) != EOF) {
        putchar(ch);
    }
    fclose(ftmp->file);
}

struct flder *filetofolder(struct flder* nfldr,int subfindex){
    struct file* ftmp=nfldr->subf[subfindex].subfile;
    if(ftmp==NULL){
        ftmp=createfile(nfldr,subfindex);
    }
    int choice;
    scanf("%d", &choice);
    if (choice == 1 && (ftmp->fperms>=4)) {
        printcontent(nfldr,subfindex);
    } else if (choice == 2 && (nfldr->perms & 2)) {
        int schoice;
        scanf("1:Append/write or 0:Truncate/write -->%d",&schoice);
        if(schoice==0){
            remove(ftmp->fname);
            writetofile(nfldr,subfindex);
        }
        else if(schoice==1){
            writetofile(nfldr,subfindex);

        }
        else{
            printf("Invalid");
        }
    } else if (choice == 3 && (ftmp->fperms>=6)) {
        deleteFile(nfldr,subfindex);
        } 
        else if (choice == 4 && (ftmp->fperms & 1)) {
        int exechoice;
        scanf("%d\n",&exechoice);
        const char ifile[max];
        scanf("%s\n",ifile);
        if (exechoice == 1) {
            execpy(ifile);
        } else if (exechoice == 2) {
            executec(ifile);
        } else if (exechoice == 3) {
            executebash(ifile);
        } else {
            printf("invalid\n");
        }
    } else {
        printf("invalid\n");
    }
}

void search(const char* name, struct flder* folder) {
    if (strcmp(folder->name, name) == 0) {
        printf("found folder: %s\n", name);
    }
    for (int i = 0; folder->subf[i].subflder != NULL; i++) {
        search(name, folder->subf[i].subflder);
    }
    for (int j = 0; folder->subf[j].subfile != NULL; j++) {
        if (strcmp(folder->subf[j].subfile->fname, name) == 0) {
            printf("found file: %s\n", name);
        }
    }
}

long sizecalc(struct flder *folder) {
    if (folder == NULL) {
        return 0;
    }
    if (folder->subf->subfile) {
        return folder->subf->subfile->fperms;
    } else if (folder->subf->subflder) {
        long size = 0;
        struct flder* current = folder->subf->subflder;
        while (current) {
            size += sizecalc(current);
            current = current->subf->subflder;
        }
        return size;
    } else {
        return 0;
    }
}

int sizecompasc(const void *a, const void *b) {
    struct flder *folderA = *((struct flder **)a);
    struct flder *folderB = *((struct flder **)b);
    long sizeA = sizecalc(folderA);
    long sizeB = sizecalc(folderB);
    return sizeA - sizeB;
}

int sizecompdsc(const void *a, const void *b) {
    struct flder *folderA = *((struct flder **)a);
    struct flder *folderB = *((struct flder **)b);
    long sizeA = sizecalc(folderA);
    long sizeB = sizecalc(folderB);
    return sizeB - sizeA;
}

void sortst(struct flder *folder) {
    if (folder == NULL || folder->subf == NULL) {
        return;
    }
    if (folder->subf->subflder) {
        int numSubfolders = 0;
        struct flder *current = folder->subf->subflder;
        while (current) {
            numSubfolders++;
            current = current->subf->subflder;
        }
        struct flder **subfolders = (struct flder **)malloc(numSubfolders * sizeof(struct flder *));
        current = folder->subf->subflder;
        int i = 0;
        while (current) {
            subfolders[i] = current;
            current = current->subf->subflder;
            i++;
        }
        int choice;
        scanf("Enter choice:",&choice);
        if(choice==0){
        qsort(subfolders, numSubfolders, sizeof(struct flder *), sizecompasc);
        folder->subf->subflder = subfolders[0];
        current = folder->subf->subflder;
        for (i = 1; i < numSubfolders; i++) {
            current->subf->subflder = subfolders[i];
            current = current->subf->subflder;
        }}
        else if(choice==1){
        qsort(subfolders, numSubfolders, sizeof(struct flder *), sizecompdsc);
        folder->subf->subflder = subfolders[0];
        current = folder->subf->subflder;
        for (i = 1; i < numSubfolders; i++) {
            current->subf->subflder = subfolders[i];
            current = current->subf->subflder;
        }}
        else{
            printf("Invalid");
        }
        free(subfolders);
    }
}

void filemove(struct flder* src, struct flder* dest, int isrc, int idest) {
    dest->subf[idest].subfile = src->subf[isrc].subfile;
    src->subf[isrc].subfile = NULL;
}

void copyFile(struct file* nfile, struct flder* dest) {
    int i=0;
    while(dest->subf[i].subfile != NULL && dest->subf[i].subflder != NULL){
        i++;
    }
    dest->subf[i].subfile = (struct file*)malloc(sizeof(struct file));
    dest->subf[i].subfile=nfile;
}

void copyFolder(struct flder* src, struct flder* dest) {
    struct flder* nflder = createflderinflder(dest, fcount(dest));
    nflder->perms = src->perms;
    strncpy(nflder->name, src->name, max);
    nflder->timestamp = time(NULL);
    int i = 0;
    while (src->subf[i].subfile != NULL) {
        copyFile(src->subf[i].subfile, nflder);
        i++;
    }
    i = 0;
    while (src->subf[i].subflder != NULL) {
        copyFolder(src->subf[i].subflder, nflder);
        i++;
    }
}

void renameFile(struct file* nfile, char* nname) {
    strncpy(nfile->fname, nname, max);
    printf("File renamed to: %s\n", nfile->fname);
}

void renameFolder(struct flder* nflder, char* nname) {
    strncpy(nflder->name, nname, max);
    printf("Folder renamed to: %s\n", nflder->name);
}

