/*
 * VLIB API definitions 2019-09-02 14:52:52
 * Input file: dpi.api
 * Automatically generated: please edit the input file NOT this file!
 */

#include <stdbool.h>
#if defined(vl_msg_id)||defined(vl_union_id) \
    || defined(vl_printfun) ||defined(vl_endianfun) \
    || defined(vl_api_version)||defined(vl_typedefs) \
    || defined(vl_msg_name)||defined(vl_msg_name_crc_list) \
    || defined(vl_api_version_tuple)
/* ok, something was selected */
#else
#warning no content included from dpi.api
#endif

#define VL_API_PACKED(x) x __attribute__ ((packed))

/****** Message ID / handler enum ******/

#ifdef vl_msg_id
vl_msg_id(VL_API_DPI_FLOW_ADD_DEL, vl_api_dpi_flow_add_del_t_handler)
vl_msg_id(VL_API_DPI_FLOW_ADD_DEL_REPLY, vl_api_dpi_flow_add_del_reply_t_handler)
#endif
/****** Message names ******/

#ifdef vl_msg_name
vl_msg_name(vl_api_dpi_flow_add_del_t, 1)
vl_msg_name(vl_api_dpi_flow_add_del_reply_t, 1)
#endif
/****** Message name, crc list ******/

#ifdef vl_msg_name_crc_list
#define foreach_vl_msg_name_crc_dpi \
_(VL_API_DPI_FLOW_ADD_DEL, dpi_flow_add_del, 52a12407) \
_(VL_API_DPI_FLOW_ADD_DEL_REPLY, dpi_flow_add_del_reply, e467dfee) 
#endif

/****** Typedefs ******/

#ifdef vl_typedefs
#ifndef included_dpi_api
#define included_dpi_api
#ifndef _vl_api_defined_dpi_flow_add_del
#define _vl_api_defined_dpi_flow_add_del
typedef VL_API_PACKED(struct _vl_api_dpi_flow_add_del {
    u16 _vl_msg_id;
    u32 client_index;
    u32 context;
    u8 is_add;
    u8 is_ipv6;
    u8 src_ip[16];
    u8 dst_ip[16];
    u16 src_port;
    u16 dst_port;
    u32 vrf_id;
    u8 protocol;
}) vl_api_dpi_flow_add_del_t;
#endif

#ifndef _vl_api_defined_dpi_flow_add_del_reply
#define _vl_api_defined_dpi_flow_add_del_reply
typedef VL_API_PACKED(struct _vl_api_dpi_flow_add_del_reply {
    u16 _vl_msg_id;
    u32 context;
    i32 retval;
    u32 flow_id;
}) vl_api_dpi_flow_add_del_reply_t;
#endif


#endif
#endif

/****** Print functions *****/
#ifdef vl_printfun

#ifdef LP64
#define _uword_fmt "%lld"
#define _uword_cast (long long)
#else
#define _uword_fmt "%ld"
#define _uword_cast long
#endif

#ifndef _vl_api_defined_dpi_flow_add_del_t_print
#define _vl_api_defined_dpi_flow_add_del_t_print
static inline void *vl_api_dpi_flow_add_del_t_print (vl_api_dpi_flow_add_del_t *a,void *handle)
{
    vl_print(handle, "vl_api_dpi_flow_add_del_t:\n");
    vl_print(handle, "_vl_msg_id: %u\n", a->_vl_msg_id);
    vl_print(handle, "client_index: %u\n", a->client_index);
    vl_print(handle, "context: %u\n", a->context);
    vl_print(handle, "is_add: %u\n", a->is_add);
    vl_print(handle, "is_ipv6: %u\n", a->is_ipv6);
    vl_print(handle, "src_port: %u\n", a->src_port);
    vl_print(handle, "dst_port: %u\n", a->dst_port);
    vl_print(handle, "vrf_id: %u\n", a->vrf_id);
    vl_print(handle, "protocol: %u\n", a->protocol);
    return handle;
}

#endif

#ifndef _vl_api_defined_dpi_flow_add_del_reply_t_print
#define _vl_api_defined_dpi_flow_add_del_reply_t_print
static inline void *vl_api_dpi_flow_add_del_reply_t_print (vl_api_dpi_flow_add_del_reply_t *a,void *handle)
{
    vl_print(handle, "vl_api_dpi_flow_add_del_reply_t:\n");
    vl_print(handle, "_vl_msg_id: %u\n", a->_vl_msg_id);
    vl_print(handle, "context: %u\n", a->context);
    vl_print(handle, "retval: %ld\n", a->retval);
    vl_print(handle, "flow_id: %u\n", a->flow_id);
    return handle;
}

#endif


#endif /* vl_printfun */

/****** Endian swap functions *****/
#ifdef vl_endianfun

#undef clib_net_to_host_uword
#ifdef LP64
#define clib_net_to_host_uword clib_net_to_host_u64
#else
#define clib_net_to_host_uword clib_net_to_host_u32
#endif

#ifndef _vl_api_defined_dpi_flow_add_del_t_endian
#define _vl_api_defined_dpi_flow_add_del_t_endian
static inline void vl_api_dpi_flow_add_del_t_endian (vl_api_dpi_flow_add_del_t *a)
{
    a->_vl_msg_id = clib_net_to_host_u16(a->_vl_msg_id);
    a->client_index = clib_net_to_host_u32(a->client_index);
    a->context = clib_net_to_host_u32(a->context);
    /* a->is_add = a->is_add (no-op) */
    /* a->is_ipv6 = a->is_ipv6 (no-op) */
    a->src_port = clib_net_to_host_u16(a->src_port);
    a->dst_port = clib_net_to_host_u16(a->dst_port);
    a->vrf_id = clib_net_to_host_u32(a->vrf_id);
    /* a->protocol = a->protocol (no-op) */
}

#endif

#ifndef _vl_api_defined_dpi_flow_add_del_reply_t_endian
#define _vl_api_defined_dpi_flow_add_del_reply_t_endian
static inline void vl_api_dpi_flow_add_del_reply_t_endian (vl_api_dpi_flow_add_del_reply_t *a)
{
    a->_vl_msg_id = clib_net_to_host_u16(a->_vl_msg_id);
    a->context = clib_net_to_host_u32(a->context);
    a->retval = clib_net_to_host_u32(a->retval);
    a->flow_id = clib_net_to_host_u32(a->flow_id);
}

#endif


#endif /* vl_endianfun */

/****** Version tuple *****/

#ifdef vl_api_version_tuple

vl_api_version_tuple(dpi.api, 1, 0, 0)

#endif /* vl_api_version_tuple */

/****** API CRC (whole file) *****/

#ifdef vl_api_version
vl_api_version(dpi.api, 0x79814222)

#endif

