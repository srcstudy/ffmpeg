/*

 */
#include <stdio.h>
#include "libavutil/dict.h"

static void print_dict(const AVDictionary *m)
{
    AVDictionaryEntry *t = NULL;
    while ((t = av_dict_get(m, "", t, AV_DICT_IGNORE_SUFFIX)))
        printf("%s:%s   ", t->key, t->value);
    printf("\n");
}

static void test_separators(const AVDictionary *m, const char pair, const char val)
{
    AVDictionary *dict = NULL;
    char pairs[] = {pair , '\0'}; //TOOD
    char vals[]  = {val, '\0'};

    char *buffer = NULL;
    int ret;

    av_dict_copy(&dict, m, 0);
    print_dict(dict);
	//将字典里面存储的Key-value整个返回，返回时key和value之间的分隔符由参数key_val_sep指定，比如你可以传‘=’
    //然后返回的key-value对之间由参数pairs_sep分隔开。
    av_dict_get_string(dict, &buffer, val, pair);
    printf("%s\n", buffer);
    av_dict_free(&dict); //TODO 用完就free,后面可以继续用
	
	
    ret = av_dict_parse_string(&dict, buffer, vals, pairs, 0);
    printf("ret %d\n", ret);
    av_freep(&buffer);
    print_dict(dict);
    av_dict_free(&dict);
}

int main(void)
{
    AVDictionary *dict = NULL;
    AVDictionaryEntry *e;
    char *buffer = NULL;

    printf("Testing av_dict_get_string() and av_dict_parse_string()\n");
    av_dict_get_string(dict, &buffer, '=', ',');  //类似 av_dict_get?  后面2个参数是分隔符
    printf("[%s]\n", buffer);
    av_freep(&buffer);
	
    av_dict_set(&dict, "aaa", "aaa", 0);
    av_dict_set(&dict, "b,b", "bbb", 0);
    av_dict_set(&dict, "c=c", "ccc", 0);
    av_dict_set(&dict, "ddd", "d,d", 0);
    av_dict_set(&dict, "eee", "e=e", 0);
    av_dict_set(&dict, "f,f", "f=f", 0);
    av_dict_set(&dict, "g=g", "g,g", 0);
    test_separators(dict, ',', '=');
    av_dict_free(&dict);
	
	
	
	//===============
	
	
	
    av_dict_set(&dict, "aaa", "aaa", 0);
    av_dict_set(&dict, "bbb", "bbb", 0);
    av_dict_set(&dict, "ccc", "ccc", 0);
	//\,='" : \,='"
	//\,='" : \,='"
    av_dict_set(&dict, "\\,=\'\"", "\\,=\'\"", 0);  //  
	//第2个参数是key-value对之后的分隔符
	//aaa=aaa"bbb=bbb"ccc=ccc"   \\,\=\'\"=\\,\=\'\"
    test_separators(dict, '"',  '=');
	
    test_separators(dict, '\'', '=');//用单引号分开
    test_separators(dict, ',', '"');
    test_separators(dict, ',', '\'');
    test_separators(dict, '\'', '"');
    test_separators(dict, '"', '\'');
    av_dict_free(&dict);

    printf("\nTesting av_dict_set()\n");
    av_dict_set(&dict, "a", "a", 0);
	printf("\nTesting av_dict_set()111\n");
	
   // av_dict_set(&dict, "b", av_strdup("b"), AV_DICT_DONT_STRDUP_VAL);
   // av_dict_set(&dict, av_strdup("c"), "c", AV_DICT_DONT_STRDUP_KEY);
   // av_dict_set(&dict, av_strdup("d"), av_strdup("d"), AV_DICT_DONT_STRDUP_KEY | AV_DICT_DONT_STRDUP_VAL);
	printf("\nTesting av_dict_set()222\n");
	
	
	av_dict_set(&dict, "e", "e", AV_DICT_DONT_OVERWRITE);
    av_dict_set(&dict, "e", "f", AV_DICT_DONT_OVERWRITE);
    av_dict_set(&dict, "f", "f", 0);
    av_dict_set(&dict, "f", NULL, 0);
    av_dict_set(&dict, "ff", "f", 0);
    av_dict_set(&dict, "ff", "f", AV_DICT_APPEND);
    e = NULL;
    while ((e = av_dict_get(dict, "", e, AV_DICT_IGNORE_SUFFIX)))
        printf("%s %s\n", e->key, e->value);
    av_dict_free(&dict);
	printf("\nTesting av_dict_set()333\n");
	
	
	
	
	
	
    av_dict_set(&dict, NULL, "a", 0);
    av_dict_set(&dict, NULL, "b", 0);
    av_dict_get(dict, NULL, NULL, 0);
    e = NULL;
    while ((e = av_dict_get(dict, "", e, AV_DICT_IGNORE_SUFFIX)))
        printf("'%s' '%s'\n", e->key, e->value);
    av_dict_free(&dict);


    //valgrind sensible test
    printf("\nTesting av_dict_set_int()\n");
    av_dict_set_int(&dict, "1", 1, AV_DICT_DONT_STRDUP_VAL);
    //av_dict_set_int(&dict, av_strdup("2"), 2, AV_DICT_DONT_STRDUP_KEY);
    //av_dict_set_int(&dict, av_strdup("3"), 3, AV_DICT_DONT_STRDUP_KEY | AV_DICT_DONT_STRDUP_VAL);
    av_dict_set_int(&dict, "4", 4, 0);
    av_dict_set_int(&dict, "5", 5, AV_DICT_DONT_OVERWRITE);
    av_dict_set_int(&dict, "5", 6, AV_DICT_DONT_OVERWRITE);
    av_dict_set_int(&dict, "12", 1, 0);
    av_dict_set_int(&dict, "12", 2, AV_DICT_APPEND);
    e = NULL;
    while ((e = av_dict_get(dict, "", e, AV_DICT_IGNORE_SUFFIX)))
        printf("%s %s\n", e->key, e->value); //TODO 数字也用%s输出了，取出来还是数字吗？
    av_dict_free(&dict);

	
	
	
    //valgrind sensible test
    printf("\nTesting av_dict_set() with existing AVDictionaryEntry.key as key\n");
    av_dict_set(&dict, "key", "old", 0);
	
    e = av_dict_get(dict, "key", NULL, 0);
    av_dict_set(&dict, e->key, "new val OK", 0);
    e = av_dict_get(dict, "key", NULL, 0);
    printf("%s\n", e->value);
    av_dict_set(&dict, e->key, e->value, 0);
    e = av_dict_get(dict, "key", NULL, 0);
    printf("%s\n", e->value);
    av_dict_free(&dict);

    return 0;
}
