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

int sqlite3_phashionext_init(
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
