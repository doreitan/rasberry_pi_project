/*
 * @Author: Yarin Avisidris 
 * @Date: 2020-10-24 20:39:26 
 * @Last Modified by: Yarin Avisidris
 * @Last Modified time: 2020-11-07 18:28:20
 */
#include "../include/commands.h"
#include "../include/program_auxiliary.h"
#include <stack> // c++ stack.
// still WIP
enum ErrorCode commands_init(cmd_tree_node *root,FILE *cmd_file,char *file_name) {
    if(!file_name) return ERROR_FILE_BAD_NAME;
    if((cmd_file = fopen(file_name,"r")) == NULL) {
        perror("ERROR: Opening file, reason:");
        printf("ERROR: number:%d\n",errno);
        printf("ERROR opening file:%s\n",file_name);
        return ERROR_OPEN_FILE;
    }
    /*
    *before starting we must check if the file's syntax is correct, if so then we continue to read the file
    *and parse the information if not we return NULL pointer.
    */
    if(commands_check_syntax(cmd_file)==ERROR_BAD_SYNTAX) return ERROR_BAD_SYNTAX;
    char read = fgetc(cmd_file);
    struct cmd_tree_node ** iterator = &root;
    struct string cmd_str;
    struct string description_str;
    std::stack<cmd_tree_node*> cmd_stack;
    bool flag = true;
    while(read!=EOF){
        read = fgetc(cmd_file);
        switch(read) {
            case '{':
            cmd_str.size++;
            cmd_str.s = (char*)realloc(cmd_str.s,cmd_str.size*sizeof(char));
            cmd_str.s[cmd_str.size-1] = '_';

            if((*iterator) == NULL) {
            (*iterator) = (struct cmd_tree_node*)calloc(1,sizeof(struct cmd_tree_node));
            (*iterator)->sub_nodes_size++;
            }
            else {
                (*iterator)->sub_nodes[(*iterator)->sub_nodes_size-1] = (struct cmd_tree_node*)calloc(1,sizeof(struct cmd_tree_node));
                // push to stack.
                cmd_stack.push((*iterator));
                iterator = &(*iterator)->sub_nodes[(*iterator)->sub_nodes_size-1];
            }
            break;

            case '}':
            // set iterator to point to current stack, then pop stack.
            if(!cmd_stack.empty()) {
            iterator = &cmd_stack.top();
            cmd_stack.pop();
            }
            break;
            
            case ';':
            (*iterator)->commands[(*iterator)->commands_size-1]->cmd_length = cmd_str.size;
            (*iterator)->commands[(*iterator)->commands_size-1]->cmd = (char*)malloc(cmd_str.size);
            memcpy((*iterator)->commands[(*iterator)->commands_size-1]->cmd,cmd_str.s,cmd_str.size);
            // done copying the command string, now remove all chars untill we reach the char '_' or end of it.
            for(;cmd_str.size-1>=0,cmd_str.s[cmd_str.size-1]!='_';cmd_str.size--);
            cmd_str.s = (char*)realloc(cmd_str.s,cmd_str.size*sizeof(char));
            break;
            case '*':
            if(!flag) {
                (*iterator)->commands[(*iterator)->commands_size-1]->description_length=description_str.size;
                (*iterator)->commands[(*iterator)->commands_size-1]->description = (char*)malloc(description_str.size);
                memcpy((*iterator)->commands[(*iterator)->commands_size-1]->description,description_str.s,description_str.size);
                // done copying the description string, now free it.
                free(description_str.s);
                description_str.size=0;
            }
            flag=!flag;
            break;
            case '\n': case '\r': case '\t':

            break;
            default:
            //copy the char into cmd_str
            if(flag) {

            if(read==' ')break;

            cmd_str.size++;
            cmd_str.s = (char*)realloc(cmd_str.s,cmd_str.size*sizeof(char));
            cmd_str.s[cmd_str.size-1] = read;
            }
            // copy the char into desc_str
            else {
            description_str.size++;
            description_str.s = (char*)realloc(cmd_str.s,description_str.size*sizeof(char));
            description_str.s[description_str.size-1] = read;
            }
            break;

        }
    }
    fclose(cmd_file);
    return ERROR_SUCCESS;
}
enum ErrorCode commands_check_syntax(FILE *cmd_file) {


    rewind(cmd_file);
    return ERROR_SUCCESS;
}
/*
needs testing.
*/
void commands_cleanup(struct cmd_tree_node*rm) {
    for(int i=0;i<rm->commands_size;i++) {
        free(rm->commands[i]->cmd);
        free(rm->commands[i]->description);
    }
    if(rm->commands_size) {
    free(rm->commands);
    }
    for(int i=0;i<rm->sub_nodes_size;i++) {
        commands_cleanup(rm->sub_nodes[i]);
    }
    if(rm->sub_nodes_size) {
        free(rm->sub_nodes);
    }
    free(rm);
}
