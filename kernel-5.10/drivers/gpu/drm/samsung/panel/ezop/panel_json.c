// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/kallsyms.h>
#include "panel.h"
#include "panel_debug.h"
#include "maptbl.h"
#include "panel_drv.h"
#include "panel_json.h"
#include "panel_delay.h"
#include "json.h"
#include "json_writer.h"
#include "json_reader.h"

const char *pnobj_cmd_type_to_json_path(u32 type)
{
	return cmd_type_to_string(type);
}

int jsonr_ref_field(json_reader_t *r, char *ref)
{
	int err;

	JEXPECT_STREQ(r, current_token(r), "$ref"); inc_token(r);

	err = jsonr_string(r, ref);
	if (err < 0)
		return err;

	return 0;

out_free:
	return err;
}

static int json_ref_to_pnobj(char *ref, struct pnobj *pnobj)
{
	char *p;
	int type;

	p = strsep(&ref, "/");
	if (!p || strcmp(p, "#"))
		return -EINVAL;

	p = strsep(&ref, "/");
	if (!p)
		return -EINVAL;

	type = string_to_cmd_type(p);
	if (type < 0)
		return -EINVAL;

	set_pnobj_cmd_type(pnobj, type);
	set_pnobj_name(pnobj, ref);

	return 0;
}


int jsonr_ref_pnobj_object(json_reader_t *r, struct pnobj **pnobj)
{
	jsmntok_t *obj_tok;
	int err;
	char name[SZ_128];
	char *ref = name;
	struct pnobj t_pnobj, *found;

	JEXPECT_OBJECT(r, current_token(r));
	obj_tok = current_token(r); inc_token(r);
	if (!obj_tok->size) {
		*pnobj = NULL;
		return 0;
	}

	err = jsonr_ref_field(r, ref);
	if (err < 0) {
		panel_err("failed to parse ref field(%s)\n", ref);
		return err;
	}

	err = json_ref_to_pnobj(ref, &t_pnobj);
	if (err < 0) {
		panel_err("failed to ref to pnobj(%s)\n", ref);
		return err;
	}

	found = pnobj_find_by_pnobj(get_jsonr_pnobj_list(r), &t_pnobj);
	if (!found) {
		panel_err("pnobj(%s:%s) not found\n",
				get_pnobj_name(&t_pnobj),
				get_pnobj_cmd_type(&t_pnobj));
		return -EINVAL;
	}

	*pnobj = found;

	return 0;

out_free:
	return err;

}

int jsonr_ref_pnobj_field(json_reader_t *r,
		const char *prop, struct pnobj **pnobj)
{
	int err;

	JEXPECT_STREQ(r, current_token(r), prop); inc_token(r);

	err = jsonr_ref_pnobj_object(r, pnobj);
	if (err < 0)
		return err;

out_free:
	return err;
}

int jsonr_ref_function_field(json_reader_t *r,
		const char *prop, void **out)
{
	struct pnobj *pnobj;
	struct pnobj_func *pnobj_func;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	pnobj_func = pnobj_container_of(pnobj, struct pnobj_func);
	*out = (void *)pnobj_func->symaddr;

	return 0;
}

int jsonr_ref_maptbl_field(json_reader_t *r,
		const char *prop, struct maptbl **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct maptbl);

	return 0;
}

int jsonr_ref_packet_field(json_reader_t *r,
		const char *prop, struct pktinfo **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct pktinfo);

	return 0;
}

int jsonr_ref_readinfo_field(json_reader_t *r,
		const char *prop, struct rdinfo **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct rdinfo);

	return 0;
}

int jsonr_ref_resource_field(json_reader_t *r,
		const char *prop, struct resinfo **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct resinfo);

	return 0;
}

int jsonr_ref_delay_field(json_reader_t *r,
		const char *prop, struct delayinfo **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct delayinfo);

	return 0;
}

int jsonr_ref_sequence_field(json_reader_t *r,
		const char *prop, struct seqinfo **out)
{
	struct pnobj *pnobj;
	int err;

	err = jsonr_ref_pnobj_field(r, prop, &pnobj);
	if (err < 0)
		return err;

	if (!pnobj) {
		*out = NULL;
		return 0;
	}

	*out = pnobj_container_of(pnobj, struct seqinfo);

	return 0;
}

int jsonr_pnobj(json_reader_t *r, struct pnobj *pnobj)
{
	struct pnobj temp_pnobj;
	char key[SZ_128];
	char value[SZ_128];
	char name[PNOBJ_NAME_LEN];
	int err, type;

	memset(&temp_pnobj, 0, sizeof(temp_pnobj));

	JEXPECT_STREQ(r, current_token(r), "pnobj"); inc_token(r);
	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);

	err = jsonr_string_field(r, key, value);
	if (err < 0)
		return err;

	if (strcmp(key, "type"))
		return -EINVAL;

	type = string_to_cmd_type(value);
	if (type < 0) {
		panel_err("invalid type(%s:%d)\n", value, type);
		return -EINVAL;
	}

	err = jsonr_string_field(r, key, name);
	if (err < 0)
		return err;

	if (strcmp(key, "name"))
		return -EINVAL;

	pnobj_init(pnobj, type, name);

	return 0;

out_free:

	return err;
}

int jsonr_pnobj_object(json_reader_t *r, struct pnobj *pnobj)
{
	int err;

	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);
	err = jsonr_pnobj(r, pnobj);
	if (err < 0)
		return err;

	return 0;

out_free:

	return err;
}

int jsonr_maptbl(json_reader_t *r, struct maptbl *m)
{
	jsmntok_t *it_tok;
	char buf[SZ_128];
	u8 *arr = NULL;
	int err;

	if (!m) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &m->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* parsing maptbl_shape */
	JEXPECT_STREQ(r, current_token(r), "shape"); inc_token(r);
	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);
	{
		err = jsonr_uint_field(r,
				buf, &m->shape.nr_dimen);
		if (err < 0)
			goto out_free;

		/* array */
		JEXPECT_STREQ(r, current_token(r), "sz_dimen"); inc_token(r);
		JEXPECT_ARRAY(r, current_token(r));
		it_tok = current_token(r); inc_token(r);
		err = jsonr_uint_array(r,
				m->shape.sz_dimen, it_tok->size);
		if (err < 0)
			goto out_free;
	}

	/* parsing maptbl array */
	JEXPECT_STREQ(r, current_token(r), "arr"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		arr = kvmalloc(sizeof(u8) * it_tok->size, GFP_KERNEL);
		err = jsonr_u8_array(r, arr, it_tok->size);
		if (err < 0)
			goto out_free;
	}

	/* parsing maptbl_ops */
	JEXPECT_STREQ(r, current_token(r), "ops"); inc_token(r);
	JEXPECT_OBJECT(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		err = jsonr_ref_function_field(r, "init", (void **)&m->ops.init);
		if (err < 0)
			goto out_free;

		err = jsonr_ref_function_field(r, "getidx", (void **)&m->ops.getidx);
		if (err < 0)
			goto out_free;

		err = jsonr_ref_function_field(r, "copy", (void **)&m->ops.copy);
		if (err < 0)
			goto out_free;
	}

	m->arr = arr;
	maptbl_set_sizeof_copy(m,
			maptbl_get_countof_n_dimen_element(m, NDARR_1D));

	return 0;

out_free:
	free_pnobj_name(&m->base);
	kvfree(arr);

	return err;
}

int jsonr_packet_update_info(json_reader_t *r, struct pkt_update_info *pktui)
{
	char buf[SZ_128];
	int err;

	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);

	/* offset */
	err = jsonr_uint_field(r, buf, &pktui->offset);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}

	/* maptbl */
	err = jsonr_ref_maptbl_field(r, "maptbl", &pktui->maptbl);
	if (err < 0) {
		panel_err("failed to parse reference of maptbl\n");
		return err;
	}

	/* getidx function */
	err = jsonr_ref_function_field(r, "getidx", (void **)&pktui->getidx);
	if (err < 0) {
		panel_err("failed to parse reference of function\n");
		return err;
	}

	return 0;

out_free:

	return err;
}

int jsonr_packet(json_reader_t *r, struct pktinfo *pkt)
{
	jsmntok_t *it_tok;
	char buf[SZ_128];
	struct pkt_update_info *pktui = NULL;
	u8 *data = NULL;
	int i, err;

	if (!pkt) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &pkt->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* parsing packet data */
	JEXPECT_STREQ(r, current_token(r), "data"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		data = kvmalloc(sizeof(u8) * it_tok->size, GFP_KERNEL);
		err = jsonr_u8_array(r, data, it_tok->size);
		if (err < 0)
			goto out_free;
	}
	pkt->dlen = it_tok->size;

	/* offset */
	err = jsonr_uint_field(r, buf, &pkt->offset);
	if (err < 0)
		goto out_free;

	/* parsing maptbl array */
	JEXPECT_STREQ(r, current_token(r), "pktui"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		pktui = kzalloc(sizeof(*pktui) * it_tok->size, GFP_KERNEL);
		for (i = 0; i < it_tok->size; i++) {
			err = jsonr_packet_update_info(r, &pktui[i]);
			if (err < 0)
				goto out_free;
		}
		pkt->nr_pktui = it_tok->size;
	}

	/* option */
	err = jsonr_uint_field(r, buf, &pkt->option);
	if (err < 0)
		goto out_free;

	pkt->data = data;
	pkt->pktui = pktui;

	return 0;

out_free:
	free_pnobj_name(&pkt->base);
	kvfree(data);
	kfree(pktui);

	return err;
}

int jsonr_keyinfo(json_reader_t *r, struct keyinfo *key)
{
	char buf[SZ_128];
	int err;

	if (!key) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &key->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* level */
	err = jsonr_uint_field(r, buf, &key->level);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		goto out_free;
	}

	/* en */
	err = jsonr_uint_field(r, buf, &key->en);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		goto out_free;
	}

	/* packet */
	err = jsonr_ref_packet_field(r, "packet", &key->packet);
	if (err < 0) {
		panel_err("failed to parse reference of packet\n");
		goto out_free;
	}

	return 0;

out_free:
	free_pnobj_name(&key->base);
	return err;
}

int jsonr_condition(json_reader_t *r, struct condinfo *cond)
{
	char buf[SZ_128];
	int err;

	if (!cond) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &cond->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* condition function */
	err = jsonr_ref_function_field(r, "cond", (void **)&cond->cond);
	if (err < 0) {
		panel_err("failed to parse reference of function\n");
		goto out_free;
	}

	return 0;

out_free:
	free_pnobj_name(&cond->base);
	return err;
}

int jsonr_read_info(json_reader_t *r, struct rdinfo *rdi)
{
	char buf[SZ_128];
	u8 *data = NULL;
	int err;

	if (!rdi) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &rdi->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* address */
	err = jsonr_uint_field(r, buf, &rdi->addr);
	if (err < 0)
		goto out_free;

	/* offset */
	err = jsonr_uint_field(r, buf, &rdi->offset);
	if (err < 0)
		goto out_free;

	/* length */
	err = jsonr_uint_field(r, buf, &rdi->len);
	if (err < 0)
		goto out_free;

	data = kvmalloc(sizeof(u8) * rdi->len, GFP_KERNEL);
	if (!data)
		goto out_free;

	rdi->data = data;

	return 0;

out_free:
	free_pnobj_name(&rdi->base);
	kfree(data);
	return err;
}

int jsonr_resource_update_info(json_reader_t *r, struct res_update_info *resui)
{
	char buf[SZ_128];
	int err;

	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);

	/* offset */
	err = jsonr_uint_field(r, buf, &resui->offset);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}

	/* readinfo */
	err = jsonr_ref_readinfo_field(r, "rdi", &resui->rditbl);
	if (err < 0) {
		panel_err("failed to parse reference of readinfo\n");
		return err;
	}

	return 0;

out_free:
	return err;
}

int jsonr_resource(json_reader_t *r, struct resinfo *res)
{
	jsmntok_t *it_tok;
	char buf[SZ_128];
	u8 *data = NULL;
	struct res_update_info *resui = NULL;
	int i, err;

	if (!res) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &res->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* parsing packet data */
	JEXPECT_STREQ(r, current_token(r), "data"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		data = kvmalloc(sizeof(u8) * it_tok->size, GFP_KERNEL);
		err = jsonr_u8_array(r, data, it_tok->size);
		if (err < 0)
			goto out_free;
	}
	res->dlen = it_tok->size;

	/* parsing resource update info */
	JEXPECT_STREQ(r, current_token(r), "resui"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		resui = kzalloc(sizeof(*resui) * it_tok->size, GFP_KERNEL);
		for (i = 0; i < it_tok->size; i++) {
			err = jsonr_resource_update_info(r, &resui[i]);
			if (err < 0)
				goto out_free;
		}
		res->nr_resui = it_tok->size;
	}

	res->data = data;
	res->resui = resui;

	return 0;

out_free:
	free_pnobj_name(&res->base);
	kvfree(data);
	kfree(resui);

	return err;
}

int jsonr_dump_expect(json_reader_t *r, struct dump_expect *expect)
{
	char buf[SZ_128];
	char str[SZ_128];
	int err;

	JEXPECT_OBJECT(r, current_token(r)); inc_token(r);

	/* offset */
	err = jsonr_uint_field(r, buf, &expect->offset);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}

	/* mask */
	err = jsonr_u8_field(r, buf, &expect->mask);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}

	/* value */
	err = jsonr_u8_field(r, buf, &expect->value);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}

	/* msg */
	err = jsonr_string_field(r, buf, str);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		return err;
	}
	expect->msg = kstrndup(str, sizeof(str), GFP_KERNEL);

	return 0;

out_free:
	return err;
}

int jsonr_dump_expect_array(json_reader_t *r,
		const char *prop, struct dump_expect **out, unsigned int *out_size)
{
	jsmntok_t *it_tok = NULL;
	int i, err;
	struct dump_expect *expects = NULL;

	if (!prop || !out)
		return -EINVAL;

	/* parsing expects array */
	JEXPECT_STREQ(r, current_token(r), prop); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size == 0) {
		*out = NULL;
		return 0;
	}

	expects = kzalloc(sizeof(*expects) * it_tok->size, GFP_KERNEL);
	for (i = 0; i < it_tok->size; i++) {
		err = jsonr_dump_expect(r, &expects[i]);
		if (err < 0)
			goto out_free;
	}
	*out = expects;
	*out_size = it_tok->size;

	return 0;

out_free:
	if (it_tok && expects) {
		for (i = 0; i < it_tok->size; i++)
			kfree(expects[i].msg);
	}
	kfree(expects);
	return err;
}

int jsonr_dumpinfo(json_reader_t *r, struct dumpinfo *dump)
{
	char buf[SZ_128];
	int err;

	if (!dump) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &dump->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* readinfo */
	err = jsonr_ref_resource_field(r, "res", &dump->res);
	if (err < 0) {
		panel_err("failed to parse reference\n");
		goto out_free;
	}

	/* callback function */
	err = jsonr_ref_function_field(r, "callback", (void **)&dump->callback);
	if (err < 0) {
		panel_err("failed to parse reference of function\n");
		goto out_free;
	}

	err = jsonr_dump_expect_array(r, "expects",
			&dump->expects, &dump->nr_expects);
	if (err < 0) {
		panel_err("failed to parse dump expects\n");
		goto out_free;
	}

	return 0;

out_free:
	free_pnobj_name(&dump->base);
	return err;
}

int jsonr_delayinfo(json_reader_t *r, struct delayinfo *delay)
{
	char buf[SZ_128];
	int err;

	if (!delay) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &delay->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* usec */
	err = jsonr_uint_field(r, buf, &delay->usec);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		goto out_free;
	}

	/* nframe */
	err = jsonr_uint_field(r, buf, &delay->nframe);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		goto out_free;
	}

	return 0;

out_free:
	free_pnobj_name(&delay->base);
	return err;
}

int jsonr_timer_delay_begin_info(json_reader_t *r, struct timer_delay_begin_info *tdbi)
{
	char buf[SZ_128];
	int err;

	if (!tdbi) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		return err;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &tdbi->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		return err;
	}

	/* delayinfo */
	err = jsonr_ref_delay_field(r, "delay", &tdbi->delay);
	if (err < 0) {
		panel_err("failed to parse reference\n");
		goto out_free;
	}

	return 0;

out_free:
	free_pnobj_name(&tdbi->base);
	return err;
}

int jsonr_power_control(json_reader_t *r, struct pctrlinfo *pwrctrl)
{
	char buf[SZ_128];
	char str[SZ_128];
	int err;

	if (!pwrctrl) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &pwrctrl->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		goto out_free;
	}

	/* pctrl_name */
	err = jsonr_string_field(r, buf, str);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}
	pwrctrl->pctrl_name = kzalloc(strlen(str) + 1, GFP_KERNEL);
	strncpy(pwrctrl->pctrl_name, str, SZ_128);

	return 0;

out_free:
	free_pnobj_name(&pwrctrl->base);
	kfree(pwrctrl->pctrl_name);

	return err;
}

int jsonr_property(json_reader_t *r, struct propinfo *prop)
{
	char buf[SZ_128];
	char str[SZ_128];
	char *prop_name = NULL;
	int err;

	if (!prop) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &prop->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		goto out_free;
	}

	/* prop_name */
	err = jsonr_string_field(r, buf, str);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}
	prop_name = kzalloc(strlen(str) + 1, GFP_KERNEL);
	strncpy(prop_name, str, SZ_128);

	/* prop_type */
	err = jsonr_uint_field(r, buf, &prop->prop_type);
	if (err < 0) {
		panel_err("failed to parse uint\n");
		goto out_free;
	}

	if (prop->prop_type == PANEL_OBJ_PROP_TYPE_VALUE) {
		/* prop_type */
		err = jsonr_uint_field(r, buf, &prop->value);
		if (err < 0) {
			panel_err("failed to parse uint\n");
			goto out_free;
		}
	} else if (prop->prop_type == PANEL_OBJ_PROP_TYPE_STR) {
		/* prop_type */
		err = jsonr_string_field(r, buf, str);
		if (err < 0) {
			panel_err("failed to parse string\n");
			goto out_free;
		}
		prop->str = kzalloc(strlen(str) + 1, GFP_KERNEL);
		strncpy(prop->str, str, SZ_128);
	}

	prop->prop_name = prop_name;

	return 0;

out_free:
	free_pnobj_name(&prop->base);
	kfree(prop_name);

	return err;
}

int jsonr_sequence(json_reader_t *r, struct seqinfo *seq)
{
	jsmntok_t *it_tok;
	char buf[SZ_128];
	struct pnobj *pnobj;
	void **cmdtbl = NULL;
	int i, err;

	if (!seq) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &seq->base);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		goto out_free;
	}

	/* parsing resource update info */
	JEXPECT_STREQ(r, current_token(r), "cmdtbl"); inc_token(r);
	JEXPECT_ARRAY(r, current_token(r));
	it_tok = current_token(r); inc_token(r);
	if (it_tok->size > 0) {
		cmdtbl = kzalloc(sizeof(*cmdtbl) * it_tok->size, GFP_KERNEL);
		for (i = 0; i < it_tok->size; i++) {
			err = jsonr_ref_pnobj_object(r, &pnobj);
			if (err < 0)
				goto out_free;

			if (!pnobj)
				break;

			cmdtbl[i] = pnobj;
		}
		seq->size = it_tok->size;
	}
	seq->cmdtbl = cmdtbl;

	return 0;

out_free:
	free_pnobj_name(&seq->base);
	kfree(cmdtbl);

	return err;
}

int jsonr_function(json_reader_t *r, struct pnobj_func *func)
{
	char buf[SZ_128];
	struct pnobj *pnobj, t_pnobj;
	int err;

	if (!func) {
		panel_err("invalid arg\n");
		return -EINVAL;
	}

	/* entry */
	err = jsonr_string(r, buf);
	if (err < 0) {
		panel_err("failed to parse string\n");
		goto out_free;
	}

	/* parsing base */
	err = jsonr_pnobj_object(r, &t_pnobj);
	if (err < 0) {
		panel_err("failed to parse pnobj\n");
		goto out_free;
	}

	pnobj = pnobj_find_by_pnobj(get_jsonr_pnobj_list(r), &t_pnobj);
	if (!pnobj) {
		panel_err("pnobj(%s:%s) not found\n",
				get_pnobj_name(&t_pnobj),
				get_pnobj_cmd_type(&t_pnobj));
		err = -EINVAL;
		goto out_free;
	}

	memcpy(func, pnobj_container_of(
				pnobj, struct pnobj_func), sizeof(*func));

	return 0;

out_free:
	free_pnobj_name(&func->base);
	return err;
}

int jsonr_all(json_reader_t *r)
{
	jsmntok_t *top, *tok;
	char buf[SZ_128];
	int i, j, err, pnobj_cmd_type;
	struct pnobj *pos, *next;

	JEXPECT_OBJECT(r, current_token(r));
	top = current_token(r); inc_token(r);
	for (i = 0; i < top->size / 2; i++) {
		err = jsonr_string(r, buf);
		if (err < 0) {
			panel_err("failed to parse string\n");
			goto out_free;
		}

		pnobj_cmd_type = string_to_cmd_type(buf);
		if (pnobj_cmd_type < 0) {
			panel_err("invalid type(%s:%d)\n", buf, pnobj_cmd_type);
			goto out_free;
		}

		JEXPECT_OBJECT(r, current_token(r));
		tok = current_token(r); inc_token(r);
		for (j = 0; j < tok->size / 2; j++) {
			panel_dbg("%.*s\n",
					SZ_16, get_jsonr_buf(r) + current_token(r)->start);

			if (pnobj_cmd_type == CMD_TYPE_MAP) {
				struct maptbl *m = kzalloc(sizeof(*m), GFP_KERNEL);

				err = jsonr_maptbl(r, m);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&m->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_RES) {
				struct resinfo *res = kzalloc(sizeof(*res), GFP_KERNEL);

				err = jsonr_resource(r, res);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&res->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_DMP) {
				struct dumpinfo *dump = kzalloc(sizeof(*dump), GFP_KERNEL);

				err = jsonr_dumpinfo(r, dump);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&dump->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_PCTRL) {
				struct pctrlinfo *pwrctrl = kzalloc(sizeof(*pwrctrl), GFP_KERNEL);

				err = jsonr_power_control(r, pwrctrl);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&pwrctrl->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_PROP) {
				struct propinfo *prop = kzalloc(sizeof(*prop), GFP_KERNEL);

				err = jsonr_property(r, prop);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&prop->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_SEQ) {
				struct seqinfo *seq = kzalloc(sizeof(*seq), GFP_KERNEL);

				err = jsonr_sequence(r, seq);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&seq->base), get_jsonr_pnobj_list(r));
			} else if (IS_CMD_TYPE_RX_PKT(pnobj_cmd_type)) {
				struct rdinfo *rdi = kzalloc(sizeof(*rdi), GFP_KERNEL);

				err = jsonr_read_info(r, rdi);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&rdi->base), get_jsonr_pnobj_list(r));
			} else if (IS_CMD_TYPE_KEY(pnobj_cmd_type)) {
				struct keyinfo *key = kzalloc(sizeof(*key), GFP_KERNEL);

				err = jsonr_keyinfo(r, key);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&key->base), get_jsonr_pnobj_list(r));
			} else if (IS_CMD_TYPE_TX_PKT(pnobj_cmd_type)) {
				struct pktinfo *pkt = kzalloc(sizeof(*pkt), GFP_KERNEL);

				err = jsonr_packet(r, pkt);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&pkt->base), get_jsonr_pnobj_list(r));
			} else if (IS_CMD_TYPE_DELAY(pnobj_cmd_type) ||
					(pnobj_cmd_type == CMD_TYPE_TIMER_DELAY)) {
				struct delayinfo *delay = kzalloc(sizeof(*delay), GFP_KERNEL);

				err = jsonr_delayinfo(r, delay);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&delay->base), get_jsonr_pnobj_list(r));
			} else if (pnobj_cmd_type == CMD_TYPE_TIMER_DELAY_BEGIN) {
				struct timer_delay_begin_info *tdbi = kzalloc(sizeof(*tdbi), GFP_KERNEL);

				err = jsonr_timer_delay_begin_info(r, tdbi);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&tdbi->base), get_jsonr_pnobj_list(r));
			} else if (IS_CMD_TYPE_COND(pnobj_cmd_type)) {
				struct condinfo *cond = kzalloc(sizeof(*cond), GFP_KERNEL);

				err = jsonr_condition(r, cond);
				if (err < 0) {
					panel_err("failed to parse\n");
					goto out_free;
				}

				list_add_tail(get_pnobj_list(&cond->base), get_jsonr_pnobj_list(r));
			}
			else if (pnobj_cmd_type == CMD_TYPE_FUNC) {
				struct pnobj_func *pnobj_func =
					kzalloc(sizeof(*pnobj_func), GFP_KERNEL);

				err = jsonr_function(r, pnobj_func);
				if (err < 0) {
					panel_err("failed to parse\n");
					kfree(pnobj_func);
					goto out_free;
				}

				kfree(pnobj_func);
			}
		}
	}

	panel_info("setup pnobj_list successfully\n");

	return 0;

out_free:
	list_for_each_entry_safe(pos, next, get_jsonr_pnobj_list(r), list) {
		list_del(&pos->list);
		destroy_panel_object(pos);
	}

	return err;
}

__visible_for_testing void jsonw_pnobj(json_writer_t *w, struct pnobj *pnobj)
{
	jsonw_name(w, "pnobj");
	jsonw_start_object(w);
	{
		jsonw_string_field(w, "type",
				cmd_type_to_string(get_pnobj_cmd_type(pnobj)));
		jsonw_string_field(w, "name", pnobj->name);
	}
	jsonw_end_object(w);
}

static void jsonw_ref(json_writer_t *w,
		const char *path, const char *key)
{
	char refname[KSYM_NAME_LEN + 32];

	jsonw_start_object(w);
	{
		snprintf(refname, sizeof(refname), "#/%s/%s", path, key);
		jsonw_string_field(w, "$ref", refname);
	}
	jsonw_end_object(w);
}

static void jsonw_ref_field(json_writer_t *w,
		const char *prop, const char *path, const char *key)
{
	jsonw_name(w, prop);
	jsonw_ref(w, path, key);
}

static void jsonw_ref_function_field(json_writer_t *w,
		const char *prop, void *f)
{
	char symname[KSYM_NAME_LEN];

	if (!f) {
		panel_err("function is null\n");
		jsonw_null_object_field(w, prop);
		return;
	}

	sprint_symbol_no_offset(symname, (unsigned long)f);
	if (!strncmp(symname, "0x", 2)) {
		panel_err("failed to lookup symbol name(%lx)\n", f);
		jsonw_null_object_field(w, prop);
		return;
	}

	jsonw_ref_field(w, prop, "FUNCTION", symname);
}

static void jsonw_ref_pnobj_field(json_writer_t *w,
		const char *prop, struct pnobj *pnobj)
{
	if (!pnobj) {
		jsonw_null_object_field(w, prop);
		return;
	}

	jsonw_ref_field(w, prop, pnobj_cmd_type_to_json_path(
				get_pnobj_cmd_type(pnobj)), pnobj->name);
}

static void jsonw_maptbl_shape(json_writer_t *w, struct maptbl *m)
{
	size_t i;

	jsonw_name(w, "shape");
	jsonw_start_object(w);
	{
		jsonw_int_field(w, "nr_dimen", maptbl_get_countof_dimen(m));
		jsonw_name(w, "sz_dimen");
		jsonw_start_array(w);
		maptbl_for_each_dimen(m, i)
			jsonw_int(w, maptbl_get_countof_n_dimen_element(m, i));
		jsonw_end_array(w);
	}
	jsonw_end_object(w);
}

static void jsonw_maptbl_array(json_writer_t *w, struct maptbl *m)
{
	size_t i;

	jsonw_name(w, "arr");
	jsonw_start_array(w);
	maptbl_for_each(m, i)
		jsonw_int(w, m->arr[i]);
	jsonw_end_array(w);
}

static void jsonw_maptbl_ops(json_writer_t *w, struct maptbl *m)
{
	jsonw_name(w, "ops");
	jsonw_start_object(w);
	{
		jsonw_ref_function_field(w, "init", (void *)m->ops.init);
		jsonw_ref_function_field(w, "getidx", (void *)m->ops.getidx);
		jsonw_ref_function_field(w, "copy", (void *)m->ops.copy);
	}
	jsonw_end_object(w);
}

int jsonw_maptbl(json_writer_t *w, struct maptbl *m)
{
	jsonw_name(w, maptbl_get_name(m));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)m);
		jsonw_maptbl_shape(w, m);
		jsonw_maptbl_array(w, m);
		jsonw_maptbl_ops(w, m);
	}
	jsonw_end_object(w);

	return 0;
}

static int jsonw_packet_update_info(json_writer_t *w, struct pkt_update_info *pktui)
{
	jsonw_start_object(w);
	jsonw_uint_field(w, "offset", pktui->offset);
	jsonw_ref_pnobj_field(w, "maptbl", (struct pnobj *)pktui->maptbl);
	jsonw_ref_function_field(w, "getidx", (void *)pktui->getidx);
	jsonw_end_object(w);

	return 0;
}

static int jsonw_packet_update_info_array(json_writer_t *w, struct pkt_update_info *pktui, u32 size)
{
	size_t i;

	jsonw_name(w, "pktui");
	jsonw_start_array(w);
	for (i = 0; i < size; i++)
		jsonw_packet_update_info(w, pktui + i);
	jsonw_end_array(w);

	return 0;
}

int jsonw_packet(json_writer_t *w, struct pktinfo *pkt)
{
	size_t i;

	jsonw_name(w, get_pktinfo_name(pkt));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)pkt);
		jsonw_name(w, "data");
		jsonw_start_array(w);
		for (i = 0; i < pkt->dlen; i++)
			jsonw_int(w, pkt->data[i]);
		jsonw_end_array(w);
		jsonw_uint_field(w, "offset", pkt->offset);
		jsonw_packet_update_info_array(w, pkt->pktui, pkt->nr_pktui);
		jsonw_uint_field(w, "option", pkt->option);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_keyinfo(json_writer_t *w, struct keyinfo *key)
{
	jsonw_name(w, get_key_name(key));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)key);
		jsonw_int_field(w, "level", key->level);
		jsonw_int_field(w, "en", key->en);
		jsonw_ref_pnobj_field(w, "packet", (struct pnobj *)key->packet);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_condition(json_writer_t *w, struct condinfo *c)
{
	jsonw_name(w, get_condition_name(c));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)c);
		jsonw_ref_function_field(w, "cond", (void *)c->cond);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_read_info(json_writer_t *w, struct rdinfo *rdi)
{
	jsonw_name(w, get_rdinfo_name(rdi));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)rdi);
		jsonw_uint_field(w, "addr", rdi->addr);
		jsonw_uint_field(w, "offset", rdi->offset);
		jsonw_uint_field(w, "len", rdi->len);
	}
	jsonw_end_object(w);

	return 0;
}

static int jsonw_resource_update_info(json_writer_t *w, struct res_update_info *resui)
{
	jsonw_start_object(w);
	jsonw_uint_field(w, "offset", resui->offset);
	jsonw_ref_pnobj_field(w, "rdi", (struct pnobj *)resui->rditbl);
	jsonw_end_object(w);

	return 0;
}

static int jsonw_resource_update_info_array(json_writer_t *w, struct res_update_info *resui, u32 size)
{
	size_t i;

	jsonw_name(w, "resui");
	jsonw_start_array(w);
	for (i = 0; i < size; i++)
		jsonw_resource_update_info(w, resui + i);
	jsonw_end_array(w);

	return 0;
}

int jsonw_resource(json_writer_t *w, struct resinfo *res)
{
	size_t i;

	jsonw_name(w, get_resource_name(res));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)res);
		jsonw_name(w, "data");
		jsonw_start_array(w);
		for (i = 0; i < res->dlen; i++)
			jsonw_int(w, res->data[i]);
		jsonw_end_array(w);
		jsonw_resource_update_info_array(w, res->resui, res->nr_resui);
	}
	jsonw_end_object(w);

	return 0;
}

static int jsonw_dump_expect(json_writer_t *w, struct dump_expect *expect)
{
	jsonw_start_object(w);
	jsonw_uint_field(w, "offset", expect->offset);
	jsonw_uint_field(w, "mask", expect->mask);
	jsonw_uint_field(w, "value", expect->value);
	jsonw_string_field(w, "msg", expect->msg);
	jsonw_end_object(w);

	return 0;
}

int jsonw_dumpinfo(json_writer_t *w, struct dumpinfo *di)
{
	int i;

	jsonw_name(w, get_dump_name(di));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)di);
		jsonw_ref_pnobj_field(w, "res", (struct pnobj *)di->res);
		jsonw_ref_function_field(w, "callback", (void *)di->callback);
		jsonw_name(w, "expects");
		jsonw_start_array(w);
		for (i = 0; i < di->nr_expects; i++)
			jsonw_dump_expect(w, &di->expects[i]);
		jsonw_end_array(w);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_delayinfo(json_writer_t *w, struct delayinfo *di)
{
	jsonw_name(w, get_delay_name(di));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)di);
		jsonw_uint_field(w, "usec", di->usec);
		jsonw_uint_field(w, "nframe", di->nframe);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_timer_delay_begin_info(json_writer_t *w, struct timer_delay_begin_info *tdbi)
{
	jsonw_name(w, get_timer_delay_begin_name(tdbi));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)tdbi);
		jsonw_ref_pnobj_field(w, "delay", (struct pnobj *)tdbi->delay);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_power_control(json_writer_t *w, struct pctrlinfo *pi)
{
	jsonw_name(w, get_pwrctrl_name(pi));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)pi);
		jsonw_string_field(w, "pctrl_name", pi->pctrl_name);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_property(json_writer_t *w, struct propinfo *prop)
{
	jsonw_name(w, get_prop_name(prop));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)prop);
		jsonw_string_field(w, "prop_name", prop->prop_name);
		jsonw_uint_field(w, "prop_type", prop->prop_type);
		if (prop->prop_type == PANEL_OBJ_PROP_TYPE_VALUE)
			jsonw_uint_field(w, "value", prop->value);
		else if (prop->prop_type == PANEL_OBJ_PROP_TYPE_STR)
			jsonw_string_field(w, "str", prop->str);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_sequence(json_writer_t *w, struct seqinfo *seq)
{
	size_t i;

	jsonw_name(w, get_sequence_name(seq));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)seq);
		jsonw_name(w, "cmdtbl");
		jsonw_start_array(w);
		for (i = 0; i < seq->size; i++) {
			struct pnobj *elem =
				(struct pnobj *)seq->cmdtbl[i];

			if (elem)
				jsonw_ref(w, pnobj_cmd_type_to_json_path(
							get_pnobj_cmd_type(elem)), elem->name);
			else
				jsonw_null(w);
		}
		jsonw_end_array(w);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_function(json_writer_t *w, struct pnobj_func *func)
{
	jsonw_name(w, get_pnobj_name(&func->base));
	jsonw_start_object(w);
	{
		jsonw_pnobj(w, (struct pnobj *)func);
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_pnobj_list(json_writer_t *w, u32 pnobj_cmd_type, struct list_head *pnobj_list)
{
	struct pnobj *pnobj;

	jsonw_name(w, pnobj_cmd_type_to_json_path(pnobj_cmd_type));
	jsonw_start_object(w);
	{
		list_for_each_entry(pnobj, pnobj_list, list) {
			if (pnobj_cmd_type != get_pnobj_cmd_type(pnobj))
				continue;

			if (pnobj_cmd_type == CMD_TYPE_MAP)
				jsonw_maptbl(w, (struct maptbl *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_RES)
				jsonw_resource(w, (struct resinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_DMP)
				jsonw_dumpinfo(w, (struct dumpinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_PCTRL)
				jsonw_power_control(w, (struct pctrlinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_PROP)
				jsonw_property(w, (struct propinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_SEQ)
				jsonw_sequence(w, (struct seqinfo *)pnobj);
			else if (IS_CMD_TYPE_RX_PKT(pnobj_cmd_type))
				jsonw_read_info(w, (struct rdinfo *)pnobj);
			else if (IS_CMD_TYPE_KEY(pnobj_cmd_type))
				jsonw_keyinfo(w, (struct keyinfo *)pnobj);
			else if (IS_CMD_TYPE_TX_PKT(pnobj_cmd_type))
				jsonw_packet(w, (struct pktinfo *)pnobj);
			else if (IS_CMD_TYPE_DELAY(pnobj_cmd_type) ||
					(pnobj_cmd_type == CMD_TYPE_TIMER_DELAY))
				jsonw_delayinfo(w, (struct delayinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_TIMER_DELAY_BEGIN)
				jsonw_timer_delay_begin_info(w, (struct timer_delay_begin_info *)pnobj);
			else if (IS_CMD_TYPE_COND(pnobj_cmd_type))
				jsonw_condition(w, (struct condinfo *)pnobj);
			else if (pnobj_cmd_type == CMD_TYPE_FUNC)
				jsonw_function(w, (struct pnobj_func *)pnobj);
			else {
				panel_warn("invalid panel object type(%d)\n", pnobj_cmd_type);
				continue;
			}
		}
	}
	jsonw_end_object(w);

	return 0;
}

int jsonw_maptbl_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_MAP, head);
}

int jsonw_read_info_list(json_writer_t *w, struct list_head *head)
{
	jsonw_pnobj_list(w, SPI_PKT_TYPE_RD, head);
	jsonw_pnobj_list(w, DSI_PKT_TYPE_RD, head);

	return 0;
}

int jsonw_resource_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_RES, head);
}

int jsonw_dumpinfo_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_DMP, head);
}

int jsonw_power_control_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_PCTRL, head);
}

int jsonw_property_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_PROP, head);
}

int jsonw_sequence_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_SEQ, head);
}

int jsonw_key_list(json_writer_t *w, struct list_head *head)
{
	jsonw_pnobj_list(w, CMD_TYPE_KEY, head);

	return 0;
}

int jsonw_packet_list(json_writer_t *w, struct list_head *head)
{
	jsonw_pnobj_list(w, DSI_PKT_TYPE_WR, head);
	jsonw_pnobj_list(w, DSI_PKT_TYPE_WR_COMP, head);
	jsonw_pnobj_list(w, DSI_PKT_TYPE_WR_PPS, head);
	jsonw_pnobj_list(w, DSI_PKT_TYPE_WR_SR, head);
	jsonw_pnobj_list(w, DSI_PKT_TYPE_WR_SR_FAST, head);

	return 0;
}

int jsonw_delay_list(json_writer_t *w, struct list_head *head)
{
	jsonw_pnobj_list(w, CMD_TYPE_DELAY, head);
	jsonw_pnobj_list(w, CMD_TYPE_VSYNC_DELAY, head);
	jsonw_pnobj_list(w, CMD_TYPE_TIMER_DELAY, head);
	jsonw_pnobj_list(w, CMD_TYPE_TIMER_DELAY_BEGIN, head);

	return 0;
}

int jsonw_condition_list(json_writer_t *w, struct list_head *head)
{
	jsonw_pnobj_list(w, CMD_TYPE_COND_IF, head);
	jsonw_pnobj_list(w, CMD_TYPE_COND_EL, head);
	jsonw_pnobj_list(w, CMD_TYPE_COND_FI, head);

	return 0;
}

int jsonw_function_list(json_writer_t *w, struct list_head *head)
{
	return jsonw_pnobj_list(w, CMD_TYPE_FUNC, head);
}

MODULE_DESCRIPTION("panel object json read/write driver");
MODULE_LICENSE("GPL");
