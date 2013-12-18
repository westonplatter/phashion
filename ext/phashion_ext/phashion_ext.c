#include "ruby.h"
#include "pHash.h"

static VALUE image_hash_for(VALUE self, VALUE _filename) {
    char * filename = StringValuePtr(_filename);
    ulong64 hash;
    if (-1 == ph_dct_imagehash(filename, hash)) {
      rb_raise(rb_eRuntimeError, "Unknown pHash error");
    }
    return ULL2NUM(hash);
}


static VALUE hamming_distance(VALUE self, VALUE a, VALUE b) {
    int result = 0;
    result = ph_hamming_distance(NUM2ULL(a), NUM2ULL(b));
    if (-1 == result) {
      rb_raise(rb_eRuntimeError, "Unknown pHash error");
    }
    return INT2NUM(result);
}

#ifdef __cplusplus
extern "C" {
#endif
  void Init_phashion_ext() {
    VALUE c = rb_cObject;
    c = rb_const_get(c, rb_intern("Phashion"));

    rb_define_singleton_method(c, "hamming_distance", (VALUE(*)(ANYARGS))hamming_distance, 2);
    rb_define_singleton_method(c, "image_hash_for", (VALUE(*)(ANYARGS))image_hash_for, 1);
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
