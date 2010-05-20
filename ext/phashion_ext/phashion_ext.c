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
#ifdef __cplusplus
}
#endif
