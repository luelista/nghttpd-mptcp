#ifndef MPTCP_RBS_H
#ifdef __cplusplus
extern "C" {
#endif

#define MPTCP_RBS_H

//#define MPTCP_DEBUG


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

extern const char* SKB_CONTENT_TYPES[];
extern const char* RBS_PROP_NAMES[];


#define SKB_CONTENT_NOSTREAM  1
// .html .htm
#define SKB_CONTENT_DOCUMENT  2
// .js
#define SKB_CONTENT_SCRIPT    3
// .css
#define SKB_CONTENT_STYLE     4
// .jpg .png .gif
#define SKB_CONTENT_IMAGE     5
// all other filetypes
#define SKB_CONTENT_OTHER     6


#define RBS_PROP_COUNTER       0
#define RBS_PROP_FRAMETYPE     1
#define RBS_PROP_STREAMID      2
#define RBS_PROP_CONTENTTYPE   3
#define RBS_PROP_CLIENT        4


unsigned int skb_prop_peek(struct skb_prop_range **list);
void skb_prop_pop(struct skb_prop_range **list, unsigned int bytes);
void skb_prop_push(struct skb_prop_range **list, unsigned int bytes, unsigned int skbProp);





#ifdef __cplusplus
} //extern C
#endif
#endif
