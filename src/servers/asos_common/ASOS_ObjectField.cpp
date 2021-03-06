#include "ASOS_ObjectField.hpp"
#include <stdio.h>
#include <string.h>

ASOS_ObjectField::ASOS_ObjectField(){
}

ASOS_ObjectField::~ASOS_ObjectField(){
  std::map<std::string, ASOS_Object *>::iterator iter;

  for(iter = object_map.begin(); iter != object_map.end(); iter++){
    printf("##### Object id = %s, p = %p\n", iter->first.c_str(), iter->second);
    delete iter->second;
  }
}

int ASOS_ObjectField::AddObject(ASOS_Object *in_object, ASOS_Node *in_node){
  char object_id[257+32+1];

  strncpy(object_id, in_object->object_field_extension, 32);
  object_id[32] = ':';
  strncpy(object_id+33, in_object->object_id, in_object->object_id_size);
  object_id[33+in_object->object_id_size] = '\0';

  object_map.insert( std::map<std::string, ASOS_Object *>::value_type(object_id , in_object) );
}

int ASOS_ObjectField::RemoveObject(ASOS_Object *in_object){
  char object_id[257+32+1];

  strncpy(object_id, in_object->object_field_extension, 32);
  object_id[32] = ':';
  strncpy(object_id+33, in_object->object_id, in_object->object_id_size);
  object_id[33+in_object->object_id_size] = '\0';

  object_map.erase( object_id );
}

ASOS_Object *ASOS_ObjectField::FindObject(char *in_field_extention, char *in_object_id, int object_id_length){
  char object_id[257+32+1];
  if(object_id_length < 256){
    strncpy(object_id, in_field_extention, 32);
    object_id[32] = ':';
    strncpy(object_id+33, in_object_id, object_id_length);
    object_id[33+object_id_length] = '\0';
  }else{
    printf("Error: object id length too long\n");
    return NULL;
  }

  std::map<std::string, ASOS_Object *>::iterator iter;
  iter = object_map.find(object_id);

  if(iter != object_map.end()){
    return iter->second;
  }else{
    return NULL;
  }
}

void ASOS_ObjectField::CleanUpByNodeLeaving(ASOS_Node *in_node){
  printf("Leave node %p on %s\n", in_node, field_id);

  //  **** [TODO] Add clean up process implementation **** 


}
