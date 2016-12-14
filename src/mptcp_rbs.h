#ifndef MPTCP_RBS_H
#ifdef __cplusplus
extern "C" {
#endif

#define MPTCP_RBS_H

#define MPTCP_RULE_SET 44
#define MPTCP_RULE_REG 45

#define MPTCP_RBS_SKB_PROPERTIES 46

struct rbs_rule_value {
  unsigned int reg_num;
  unsigned int value;
};

struct skb_prop_range {
  struct skb_prop_range* next;
  unsigned int frameLength;
  unsigned int skbProperty;
};

bool rbs_load_rule_set(const char *name, const char *rules);
bool rbs_remove_rule_set(const char *name);
bool rbs_set_reg(int sock, unsigned int reg_num, unsigned int value);
bool rbs_set_skb_property(int sock, unsigned int value);
bool rbs_select_ruleset(int sock, char* name);



unsigned int skb_prop_peek(struct skb_prop_range **list);
void skb_prop_pop(struct skb_prop_range **list, unsigned int bytes);
void skb_prop_push(struct skb_prop_range **list, unsigned int bytes, unsigned int skbProp);

#ifdef __cplusplus
} //extern C
#endif
#endif
