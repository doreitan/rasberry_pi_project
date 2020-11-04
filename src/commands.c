/*
 * @Author: Yarin Avisidris 
 * @Date: 2020-10-24 20:39:26 
 * @Last Modified by: Yarin Avisidris
 * @Last Modified time: 2020-10-29 01:43:25
 */
#include "../include/commands.h"
#include "../include/program_auxiliary.h"
// still WIP
struct cmd_tree_node * read_commands(FILE *file,char *file_name) {
    if(!file_name) return NULL;
    struct cmd_tree_node *out = (struct cmd_tree_node*)calloc(1,sizeof(struct cmd_tree_node));
    if(file = fopen(file_name,"r") == NULL) {
        perror("ERROR: Opening file, reason:");
        printf("ERROR: number:%d\n",errno);
        printf("ERROR opening file:%s\n",file_name);
        return NULL;
    }
    char read;
    while((read = fgetc(file)) !=EOF) {
        switch(read) {
            case '{':

            break;
            case ';':

            break;

            case ' ':
            break;

        }
    }
    fclose(file);   
}
/*
needs testing.
*/
void commands_free_memory(struct cmd_tree_node*rm) {
    for(int i=0;i<rm->commands_size;i++) {
        free(rm->commands[i]);
    }
    if(rm->commands_size) {
    rm->commands_size = 0;
    free_and_null_2(&rm->commands);
    }
    for(int i=0;i<rm->sub_nodes_size;i++) {
        commands_free_memory(rm->sub_nodes[i]);
    }
    if(rm->sub_nodes_size) {
        rm->sub_nodes_size = 0;
        free_and_null_2(&rm->sub_nodes);
    }
}
