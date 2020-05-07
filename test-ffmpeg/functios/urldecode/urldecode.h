/*
 * not public api
 */

#ifndef AVFORMAT_URLDECODE_H
#define AVFORMAT_URLDECODE_H

/**
 * Decodes an URL from its percent-encoded form back into normal
 * representation. This function returns the decoded URL in a string.
 * The URL to be decoded does not necessarily have to be encoded but
 * in that case the original string is duplicated.
 *
 * @param url a string to be decoded.
 * @return new string with the URL decoded or NULL if decoding failed.
 * Note that the returned string should be explicitly freed when not
 * used anymore.
  url没有编码也可以解码，失败返回NULL，不用后需要释放av_free
 */
char *ff_urldecode(const char *url);

#endif /* AVFORMAT_URLDECODE_H */
