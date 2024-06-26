#pragma once

// As defined in the thumbnailer spec.
enum ErrorCode {
	ERROR_UNKNOWN_SCHEME_OR_MIME,
	ERROR_SPECIALIZED_THUMBNAILER_CONNECTION_FAILED,
	ERROR_INVALID_DATA,
	ERROR_THUMBNAIILING_THUMBNAIL,
	ERROR_COULD_NOT_WRITE,
	ERROR_UNSUPPORTED_FLAVOR,
};

struct _ThumbnailerSpecializedThumbnailer1;
extern struct _ThumbnailerSpecializedThumbnailer1 *thumbnailer_interface;
