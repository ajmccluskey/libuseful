#include "test_utils.h"
#include "stringbuilder.h"

static void _sb_info(FILE* out, stringbuilder* sb)  {
    fprintf(out, "sb(%p) cstr: %p  pos: %d  size: %d  reallocs: %d\n", 
        sb, sb->cstr, sb->pos, sb->size, sb->reallocs);
}

static void _assert_sb_stats(stringbuilder* sb, const char* str, int size, int reallocs)    {
    if (str && strcmp(sb_cstring(sb), str)) {
        fprintf(stderr, "SB string (%s) does not match '%s'\n", sb_cstring(sb), str);
        exit(-1);
    }
    
    if (size != sb->size)   {
        fprintf(stderr, "SB Size (%d) does not match required size (%d)\n", sb->size, size);
        exit(-1);
    }
    
    if (reallocs != sb->reallocs)   {
        fprintf(stderr, "SB Reallocs (%d) do not match required reallocs (%d)\n", sb->reallocs, reallocs);
        exit(-1);
    }
}

DEFINE_TEST_FUNCTION {  
    char *cstr;
    stringbuilder* sb = sb_new_with_size(1);
    
    sb_append_ch(sb, 'H');
    sb_append_ch(sb, 'e');
    sb_append_ch(sb, 'l');
    sb_append_ch(sb, 'l');
    sb_append_ch(sb, 'o');
    sb_append_ch(sb, ',');
    sb_append_ch(sb, ' ');
    sb_append_ch(sb, 'W');
    sb_append_ch(sb, 'o');
    sb_append_ch(sb, 'r');
    sb_append_ch(sb, 'l');
    sb_append_ch(sb, 'd');
    sb_append_ch(sb, '!');
    
    _assert_sb_stats(sb, "Hello, World!", strlen("Hello, World!")+1, 7);
    
    cstr = sb_make_cstring(sb);
    if (strcmp(cstr, "Hello, World!"))  {
        fprintf(stderr, "CSTR (%s) does not equal SB (%s)\n", cstr, sb_cstring(sb));
        exit(-1);
    }
    free(cstr);
        
    sb_reset(sb);
    
    sb_append_ch(sb, 'H');
    sb_append_ch(sb, 'i');
    sb_append_ch(sb, '!');
    _assert_sb_stats(sb, "Hi!", strlen("Hello, World!")+1, 7);
    
    sb_append_str(sb, "This is a longer string that I am appending, doncha know");
    _assert_sb_stats(sb, "Hi!This is a longer string that I am appending, doncha know", 71, 8);
    
    sb_append_ch(sb, '?');
    _assert_sb_stats(sb, "Hi!This is a longer string that I am appending, doncha know?", 71, 8);
    
    sb_append_strf(sb, " And %s %s!", "even", "longer");
    _assert_sb_stats(sb, "Hi!This is a longer string that I am appending, doncha know? And even longer!", 79, 9);
    
    sb_destroy(sb, 1);
    return 0;
}

int main(int argc, char** argv) {
    RUN_TEST;
}