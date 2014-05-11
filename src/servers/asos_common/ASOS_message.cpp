#include <stdio.h>
#include "ASOS_message.hpp"

ASOS_message::ASOS_message(){
  object_field_identification = NULL;
  object_identification = NULL; 
  payload = NULL;
  model_data = NULL;
  message = NULL;
}

ASOS_message::~ASOS_message(){
}

const char *ASOS_message::message_type_string(){
  switch(message_type){
  case 0x00:  return "object heart beat";
  case 0x01:  return "model publish";
  case 0x02:  return "ping object command";
  case 0x82:  return "ping object response";
  case 0x03:  return "browse model command";
  case 0x83:  return "browse model response";
  case 0x04:  return "update model command";
  case 0x84:  return "update model response";
  case 0x05:  return "pop message command";
  case 0x85:  return "pop message response";
  case 0x06:  return "push message command";
  case 0x86:  return "push message response";
  case 0x07:  return "create object command";
  case 0x87:  return "create object response";
  case 0x08:  return "register object heartbeat command";
  case 0x88:  return "register object heartbeat response";
  case 0x09:  return "register model subscription command";
  case 0x89:  return "register model subscription response";
  case 0x0a:  return "delete object command";
  case 0x8a:  return "delete object response";
  case 0x0b:  return "cancel object heartbeat command";
  case 0x8b:  return "cancel object heartbeat response";
  case 0x0c:  return "cancel model subscription command";
  case 0x8c:  return "cancel model subscription response";
  default:    return "(unknown)";
  }
}

void ASOS_message::print(){
  int i;
  printf("message type            : (0x%02x) %s\n", message_type, message_type_string() );
  printf("wait time for response  : %d [sec]\n"   , wait_time_for_response );
  printf("registration lifetime   : %d [min]\n"   , registration_lifetime );
  printf("persistency flag        : %s\n"         , (persistency_flag==0x1)?"true":"false" );
  printf("lifetime overwrite flag : %s\n"         , (lifetime_overwrite_flag==0x1)?"true":"false" );
  printf("message identification  : %02x%02x\n"   , message_identification[0], message_identification[1] );
  printf("object field id length  : %d\n"         , object_field_identification_length );
  printf("object id length        : %d\n"         , object_identification_length );
  printf("payload length          : %d\n"         , payload_size );

  printf("object field id         : ");
  if(object_field_identification != NULL && object_field_identification_length > 0){
    for(i=0; i<object_field_identification_length; i++){
      printf("%c", object_field_identification[i]);
    }
    printf("\n");
  }else{
    printf("(none)\n");
  }

  printf("object id               : ");
  if(object_identification != NULL && object_identification_length > 0){
    for(i=0; i<object_identification_length; i++){
      printf("%c", object_identification[i]);
    }
    printf("\n");
  }else{
    printf("(none)\n");
  }

  printf("payload                 : ");
  if(payload != NULL && payload_size > 0){
    for(i=0; i<payload_size; i++){
      printf("%02x", payload[i]);
    }
    printf("\n");
  }else{
    printf("(none)\n");
  }

  // ---------------------------------------------------
  // print payload 

  if(message_type == 0x02 || message_type == 0x05 || message_type == 0x07 || message_type == 0x08 || message_type == 0x09
     || message_type == 0x0a || message_type == 0x0b  || message_type == 0x0c ){
    printf("[none]\n"); return;
  }

  if(message_type == 0x82 || message_type == 0x83 || message_type == 0x84 || message_type == 0x85 || message_type == 0x86
     || message_type == 0x87 || message_type == 0x88 || message_type == 0x89 
     || message_type == 0x8a || message_type == 0x8b  || message_type == 0x8c ){
    printf("[response state]     (%02x) %s\n", response_state, "** todo: make string for response state ** "); 
  }

  if(message_type == 0x00 || message_type == 0x82 ){
    printf("[object state]       (%02x) %s\n", object_state, "** todo: make string for object state ** "); 
  }

  if(message_type == 0x01 || message_type == 0x83 || message_type == 0x04 || message_type == 0x85 || message_type == 0x06
     || message_type == 0x87 ){
    printf("[model revosion]     %lld\n", model_revision); 
  }
  
  if(message_type == 0x01 || message_type == 0x83 || message_type == 0x04 ){
    printf("[model data (%d)]    ", model_data_size);
    for(i=0; i<model_data_size; i++) printf("%c", model_data[i]);
    printf("\n");
  }

  if(message_type == 0x85 || message_type == 0x06 ){
    printf("[message (%d)]       ", message_size);
    for(i=0; i<message_size; i++) printf("%c", message[i]);
    printf("\n");
  }

}

long long int ASOS_message::get_revision_from_net(const unsigned char *buff){
  long long int ret = 0;

  ret  = ((long long int)buff[0]) * 0x0100000000000000 ;
  ret += ((long long int)buff[1]) * 0x0001000000000000 ;
  ret += ((long long int)buff[2]) * 0x0000010000000000 ;
  ret += ((long long int)buff[3]) * 0x0000000100000000 ;
  ret += ((long long int)buff[4]) * 0x0000000001000000 ;
  ret += ((long long int)buff[5]) * 0x0000000000010000 ;
  ret += ((long long int)buff[6]) * 0x0000000000000100 ;
  ret += ((long long int)buff[7]) * 0x0000000000000001 ;

  return ret;
}
