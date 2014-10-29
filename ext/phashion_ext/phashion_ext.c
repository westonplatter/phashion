#include "ruby.h"
#include "pHash.h"
#ifdef HAVE_RUBY_THREAD_H
#include <ruby/thread.h>
#else
#include <ruby/intern.h>
void *
rb_thread_call_without_gvl(void *(*func)(void *data), void *data1,
			    rb_unblock_function_t *ubf, void *data2) {
  return (void *)rb_thread_blocking_region(
      (rb_blocking_function_t *)func, data1,
      (rb_unblock_function_t *)ubf, data2);
}
#endif

struct nogvl_hash_args {
  const char * filename;
  ulong64 hash;
  int retval;
};

static void * nogvl_hash(struct nogvl_hash_args * args) {
  ulong64 hash;

  args->retval = ph_dct_imagehash(args->filename, hash);
  args->hash = hash;

  return NULL;
}

static VALUE image_hash_for(VALUE self, VALUE _filename) {
    ulong64 hash;
    struct nogvl_hash_args args;

    args.filename = StringValuePtr(_filename);
    args.retval = -1;

    rb_thread_call_without_gvl((void *(*)(void *))nogvl_hash,
        (void *)&args, RUBY_UBF_PROCESS, 0);

    if (-1 == args.retval) {
      rb_raise(rb_eRuntimeError, "Unknown pHash error");
    }
    return ULL2NUM(args.hash);
}


static VALUE hamming_distance(VALUE self, VALUE a, VALUE b) {
    int result = 0;
    result = ph_hamming_distance(NUM2ULL(a), NUM2ULL(b));
    if (-1 == result) {
      rb_raise(rb_eRuntimeError, "Unknown pHash error");
    }
    return INT2NUM(result);
}

static VALUE hamming_distance2(VALUE self, VALUE a, VALUE b) {
	double distance;
	uint8_t* hashA;
	uint8_t* hashB;
	int lenA = RARRAY_LEN(a);
	int lenB = RARRAY_LEN(b);

	hashA = (uint8_t*)xcalloc(lenA, sizeof(uint8_t));
	hashB = (uint8_t*)xcalloc(lenB, sizeof(uint8_t));

	for(int i = 0; i < lenA; i++) {
		hashA[i] = NUM2INT(rb_ary_entry(a, i));
	}
	for(int i = 0; i < lenB; i++) {
		hashB[i] = NUM2INT(rb_ary_entry(b, i));
	}

	distance = ph_hammingdistance2(hashA, lenA, hashB, lenB);

	xfree(hashA);
	xfree(hashB);

	return DBL2NUM(distance);
}

static VALUE mh_hash_for(VALUE self, VALUE filename, VALUE alpha, VALUE lvl) {
	uint8_t* result;
	int n;
	VALUE array;
	result = ph_mh_imagehash(StringValuePtr(filename),
			n,
			NUM2DBL(alpha),
			NUM2DBL(lvl));
	array = rb_ary_new2(n);

	for(int i = 0; i < n; i++) {
		rb_ary_push(array, INT2FIX(result[i]));
	}

	xfree(result);

	return array;
}

static VALUE texthash_for(VALUE self, VALUE file) {
    int nbpoints, i;
    VALUE list;
    VALUE point_class;

    TxtHashPoint *points = ph_texthash(StringValuePtr(file), &nbpoints);

    point_class = rb_const_get(self, rb_intern("TextHashPoint"));

    list = rb_ary_new2((long)nbpoints);

    for(i = 0; i < nbpoints; i++) {
      VALUE point;
      VALUE args[2];

      args[0] = ULL2NUM(points[i].hash);
      args[1] = ULL2NUM(points[i].index);

      point = rb_class_new_instance(2, args, point_class);
      rb_ary_push(list, point);
    }

    free(points);

    return list;
}

static TxtHashPoint * rb2phash_points(VALUE list) {
    int i;
    TxtHashPoint * txt_list;

    txt_list = (TxtHashPoint *)xcalloc(RARRAY_LEN(list), sizeof(TxtHashPoint));

    for(i = 0; i < RARRAY_LEN(list); i++) {
      VALUE elem = rb_ary_entry(list, i);
      txt_list[i].hash = NUM2ULL(rb_funcall(elem, rb_intern("hash"), 0));
      txt_list[i].index = NUM2INT(rb_funcall(elem, rb_intern("index"), 0));
    }

    return txt_list;
}

static VALUE textmatches_for(VALUE self, VALUE list1, VALUE list2) {
    int nbmatches, i;
    VALUE list;
    VALUE match_class;
    TxtHashPoint *txt_list1;
    TxtHashPoint *txt_list2;

    txt_list1 = rb2phash_points(list1);
    txt_list2 = rb2phash_points(list2);

    TxtMatch *matches = ph_compare_text_hashes(txt_list1, RARRAY_LEN(list1),
                                               txt_list2, RARRAY_LEN(list2),
                                               &nbmatches);

    xfree(txt_list1);
    xfree(txt_list2);

    match_class = rb_const_get(self, rb_intern("TextMatch"));

    list = rb_ary_new2((long)nbmatches);

    for(i = 0; i < nbmatches; i++) {
      VALUE match;
      VALUE args[3];

      args[0] = INT2NUM(matches[i].first_index);
      args[1] = INT2NUM(matches[i].second_index);
      args[2] = INT2NUM(matches[i].length);

      match = rb_class_new_instance(3, args, match_class);
      rb_ary_push(list, match);
    }

    free(matches);

    return list;
}

#ifdef __cplusplus
extern "C" {
#endif
  void Init_phashion_ext() {
    VALUE c = rb_cObject;
    c = rb_const_get(c, rb_intern("Phashion"));

    rb_define_singleton_method(c, "hamming_distance", (VALUE(*)(ANYARGS))hamming_distance, 2);
    rb_define_singleton_method(c, "image_hash_for", (VALUE(*)(ANYARGS))image_hash_for, 1);
      
    rb_define_singleton_method(c, "_mh_hash_for", (VALUE(*)(ANYARGS))mh_hash_for, 3);
    rb_define_singleton_method(c, "hamming_distance2", (VALUE(*)(ANYARGS))hamming_distance2, 2);
      
    rb_define_singleton_method(c, "texthash_for", (VALUE(*)(ANYARGS))texthash_for, 1);
    rb_define_singleton_method(c, "textmatches_for", (VALUE(*)(ANYARGS))textmatches_for, 2);
  }

#ifdef HAVE_SQLITE3EXT_H
#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT1

static void hamming_distance(sqlite3_context * ctx, int agc, sqlite3_value **argv)
{
  sqlite3_int64 hashes[4];
  ulong64 left, right;
  int i, result;

  for(i = 0; i < 4; i++) {
    if (SQLITE_INTEGER == sqlite3_value_type(argv[i])) {
      hashes[i] = sqlite3_value_int64(argv[i]);
    } else {
      hashes[i] = 0;
    }
  }

  left = (hashes[0] << 32) + hashes[1];
  right = (hashes[2] << 32) + hashes[3];
  result = ph_hamming_distance(left, right);
  sqlite3_result_int(ctx, result);
}

int sqlite3_extension_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);

  sqlite3_create_function(
      db,
      "hamming_distance",
      4,
      SQLITE_UTF8,
      NULL,
      hamming_distance,
      NULL,
      NULL
  );
  return SQLITE_OK;
}

#endif

#ifdef __cplusplus
}
#endif
