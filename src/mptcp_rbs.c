#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "mptcp_rbs.h"

bool rbs_load_rule_set(const char *name, const char *rules)
{
  int file;
  ssize_t n = 0;
  size_t namelen = strlen(name);
  size_t ruleslen = strlen(rules);
  char *str = (char*)malloc(namelen + 2 + ruleslen + 1);

  memcpy(str, name, namelen);
  memcpy(str + namelen, "; ", 2);
  memcpy(str + namelen + 2, rules, ruleslen);
  str[namelen + 2 + ruleslen] = 0;

  file = open("/proc/net/mptcp_net/rbs/rulesets", O_WRONLY);
  if (file >= 0) {
    n = write(file, str, strlen(str));
    close(file);
  }

  free(str);
  return n > 0;
}

bool rbs_remove_rule_set(const char *name) {
  int file;
  ssize_t n;
  char path[256];

  memset(path, 0, sizeof(path));
  sprintf(path, "%s%s%s", "/proc/net/mptcp_net/rbs/", name, "/delete");

  file = open(path, O_WRONLY);
  if (file < 0)
    return false;

  n = write(file, "1", 1);
  close(file);

  return n == 1;
}

bool rbs_set_reg(int sock, unsigned int reg_num, unsigned int value) {
  struct rbs_rule_value val;
  val.reg_num = reg_num;
  val.value = value;
#ifdef MPTCP_DEBUG
  printf("rbs_set_reg: R%d = %d\n", reg_num, value);
#endif
  return !setsockopt(sock, IPPROTO_TCP, MPTCP_RULE_REG, &val, sizeof(struct rbs_rule_value));
}

bool rbs_set_skb_property(int sock, unsigned int value) {
#ifdef MPTCP_DEBUG
  printf("rbs_set_skb_property: %d\n", value);
#endif
  return !setsockopt(sock, IPPROTO_TCP, MPTCP_RBS_SKB_PROPERTIES, &value, sizeof(unsigned int));
}

bool rbs_select_ruleset(int sock, char* name) {
  if (setsockopt(sock, IPPROTO_TCP, MPTCP_RULE_SET, name, strlen(name)) < 0) {
    printf("Error: Could not set rules for socket\n");
    return false;
  } else {
    return true;
  }
}


unsigned int skb_prop_peek(struct skb_prop_range **list) {
  return (*list)->skbProperty;
}
void skb_prop_pop(struct skb_prop_range **list, unsigned int bytes) {
  struct skb_prop_range* old;
  while(bytes > 0 && bytes >= (*list)->frameLength) {
    bytes -= (*list)->frameLength;
    old = (*list);
    *list = old->next;
    free(old);
  }
  if (bytes > 0)
    (*list)->frameLength -= bytes;
}
void skb_prop_push(struct skb_prop_range **list, unsigned int bytes, unsigned int skbProp) {
  if (bytes == 0) return;
  while (*list != NULL)
    list = &(*list)->next;
  *list = (struct skb_prop_range*) malloc(sizeof(struct skb_prop_range));
  (*list)->next = NULL;
  (*list)->skbProperty = skbProp;
  (*list)->frameLength = bytes;
}


