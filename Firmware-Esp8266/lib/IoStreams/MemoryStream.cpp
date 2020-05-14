#include "MemoryStream.h"

MemoryStream::MemoryStream(uint16_t buffer_size) {
	this->buffer = (uint8_t*) malloc(buffer_size);
	this->buffer_size = buffer_size;
	this->pos = 0;
	this->size = 0;
}
MemoryStream::~MemoryStream() {
	free(buffer);
}

void MemoryStream::clear() {
	this->pos = 0;
	this->size = 0;
}

int MemoryStream::read() {
  if (size == 0) {
	return -1;
  } 
  else {
	int ret = buffer[pos];
	pos++;
	size--;
	if (pos == buffer_size) {
	  pos = 0;
	}
	return ret;
  }
}

// size_t readBytes(char *buffer, size_t length) {
// 	return -1;
// }

size_t MemoryStream::write(uint8_t v) {
  if (size == buffer_size) {
	return 0;
  } 
  else {
	int p = pos + size;
	if (p >= buffer_size) {
	  p -= buffer_size;
	}
	buffer[p] = v;
	size++;
	return 1;
  }  
}

int MemoryStream::available() {
  return size;
}

int MemoryStream::availableForWrite() {
  return buffer_size - size;
}

int MemoryStream::peek() {
  return size == 0 ? -1 : buffer[pos];
}

void MemoryStream::flush() {
}
