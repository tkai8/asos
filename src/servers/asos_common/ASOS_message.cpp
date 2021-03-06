#include <stdio.h>
#include <string.h>
#include "ASOS_message.hpp"

ASOS_message::ASOS_message(){
  object_field_identification = NULL;
  object_identification = NULL; 
  payload = NULL;
  model_data = NULL;
  message = NULL;
  long_polling_flag = 0;
  key_count = 0;
  node_id_count = 0;
  is_own = 0;
}

ASOS_message::~ASOS_message(){
}

const char *ASOS_message::message_type_string(){
  switch(message_type){
  case 0x01:  return "model publish";
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
  case 0x09:  return "register model subscription command";
  case 0x89:  return "register model subscription response";
  case 0x0a:  return "delete object command";
  case 0x8a:  return "delete object response";
  case 0x0c:  return "cancel model subscription command";
  case 0x8c:  return "cancel model subscription response";

  case 0x0d:  return "captured message";
  case 0x0e:  return "register message capture command";
  case 0x8e:  return "register message capture response";
  case 0x0f:  return "cancel message capture command";
  case 0x8f:  return "cancel message capture response";

  default:    return "(unknown)";
  }
}

const char *ASOS_message::response_state_string(){
  switch(response_state){
  case 0x00:  return "success";
  case 0x01:  return "field or object not found";
  case 0x02:  return "timeout";
  case 0x03:  return "access denied";
  case 0x04:  return "access limitation over";
  case 0x05:  return "same command already done";
  case 0x80:  return "previous model revision is too old";
  case 0x81:  return "previous model revision is invalid (future revision)";
  case 0x82:  return "model size is too big";
  case 0x83:  return "target model revision is too old";
  case 0x84:  return "target model revision is invalid (future revision)";
  case 0x85:  return "message size is too big";
  case 0x86:  return "queue is full";
  case 0x87:  return "queue is empty";
  case 0x88:  return "object already exist";
  case 0x89:  return "server resource error";
  default:    return "(unknown)";
  }
}

const char *ASOS_message::object_state_string(){
  switch(object_state){
  case 0x00:  return "object not acvite";
  case 0x01:  return "object active";
  case 0x02:  return "object busy";
  default:    return "(unknown)";
  }
}

void ASOS_message::print(ASOS_Protocolv1_info *pinfo){
  int i;
  printf("message type            : (0x%02x) %s\n", message_type, message_type_string() );
  printf("wait time for response  : %d [sec]\n"   , wait_time_for_response );
  printf("registration lifetime   : %d [min]\n"   , registration_lifetime );
  printf("lifetime overwrite flag : %s\n"         , (lifetime_overwrite_flag==0x1)?"true":"false" );
  printf("private flag            : %s\n"         , (private_flag==0x1)?"true":"false" );
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

  printf("object field extension  : ");
  if(object_field_extension != NULL){
    for(i=0; i<16; i++){
      printf("%02x", object_field_extension[i]);
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

  // -------------------------------------------------
  // get protocol info
  // -------------------------------------------------
  if(pinfo == NULL) {printf("Error: asos message type is unknown\n"); return;}

  // ---------------------------------------------------
  // print payload 
  // ---------------------------------------------------

  if(pinfo->flag_response_state == 1){
    printf("  [response state]     (%02x) %s\n", response_state, response_state_string() ); 
  }
  if(pinfo->flag_object_state == 1){
    printf("  [object state]       (%02x) %s\n", object_state, object_state_string() ); 
  }

  if(pinfo->flag_key == 1){
    printf("  [key count]          %d\n", key_count ); 
    int i, jj;
    for(i=0; i<key_count; i++){
      printf("  [key (%d)]           ", i);
      for(jj=0; jj<16; jj++) printf("%02x", key_list[i][jj]);
      printf("\n");
    }  
  }

  if(pinfo->flag_keys_and_node_ids == 1){
    printf("  [key count]          %d\n", key_count ); 
    printf("  [node id count]      %d\n", node_id_count ); 
    int i, jj;
    for(i=0; i<key_count; i++){
      printf("  [key (%d)]           ", i);
      for(jj=0; jj<16; jj++) printf("%02x", key_list[i][jj]);
      printf("\n");
    }
    for(i=0; i<node_id_count; i++){
      printf("  [node id (%d)]       ", i);
      for(jj=0; jj<16; jj++) printf("%02x", node_id_list[i][jj]);
      printf("\n");
    }
  }

  if(pinfo->flag_model_revision == 1){
    printf("  [model revosion]     %lld\n", model_revision); 
  }
  if(pinfo->flag_model_data == 1){
    printf("  [model data (%d)]    ", model_data_size);
    for(i=0; i<model_data_size; i++) printf("%c", model_data[i]);
    printf("\n");
  }
  if(pinfo->flag_message == 1){
    printf("  [message (%d)]       ", message_size);
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

void ASOS_message::set_revision_to_net(unsigned char *buff, long long int rev){
  buff[0] = ( rev / 0x0100000000000000 ) % 0x100;  
  buff[1] = ( rev / 0x0001000000000000 ) % 0x100;  
  buff[2] = ( rev / 0x0000010000000000 ) % 0x100;  
  buff[3] = ( rev / 0x0000000100000000 ) % 0x100;  
  buff[4] = ( rev / 0x0000000001000000 ) % 0x100;  
  buff[5] = ( rev / 0x0000000000010000 ) % 0x100;  
  buff[6] = ( rev / 0x0000000000000100 ) % 0x100;  
  buff[7] = ( rev / 0x0000000000000001 ) % 0x100;  
}

void ASOS_message::copy(ASOS_message *src){
  memcpy(this, src, sizeof(ASOS_message));
}

void ASOS_message::ModifyToResponse(){
  message_type |= 0x80; // cange to response
  wait_time_for_response = 0x00;
  registration_lifetime = 0x00;
  lifetime_overwrite_flag = 0x00;
  private_flag = 0x00;
  payload_size = 0;
  object_state = 0;
  model_revision = 0;
  response_state = 0x00;
  key_count = 0;
  node_id_count = 0;
  model_data_size = 0x00;
  model_data = NULL;
  message_size = 0;
  message = NULL;
}

