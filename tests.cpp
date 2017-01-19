#include <iostream>

#include <bson.h>
#include <stdlib.h>
// #include <stdio.h>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <iomanip>


#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

// Exemplary base64 encode method by Michal Lihocký
// See http://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
typedef unsigned char BYTE;

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(BYTE const* buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}

void cout_experiments(const char *name, int wiresize, double time_elapsed){
    std::cout << std::setw(40) << std::left << std::setfill(' ') << name << "ws: " << std::setw(15) << wiresize << " time(s): "<< time_elapsed << std::endl;
}


int sum(int a, int b){
  return a+b;
}

int main(int argc, const char* argv[] ){
  Timer tmr;

  const int fakedata_size = 1024 * 1024 * 6;
  char fakedata[fakedata_size]; // 6 MB of data, which is almost equal to a FullHD picture in RGB8
  std::fill_n(fakedata, fakedata_size, 120);

  auto i = 0;
  std::cout << "Some experiments on schema-less binary serialization. Comparing BSON (libbson) with RapidJSON." << std::endl;
  std::cout << "The 'Basic' usecase is a simple JSON object with tree key/value pairs:" << std::endl;
  std::cout << "  {\"a\":1,\"hello\":\"world\",\"bool\":true}" << std::endl;
  std::cout << "The 'Binary' usecase encodes a simple string key/value pair and 6 MB of data." << std::endl;
  std::cout << std::endl;
  std::cout << "ws = wire size. These is the amount of bytes that would have to be transferred over the wire. This is the length of the actual encoded data." << std::endl;
  std::cout << std::endl;
  {
    tmr.reset();
    bson_t b = BSON_INITIALIZER;

    BSON_APPEND_INT32 (&b, "a", 1);
    BSON_APPEND_UTF8 (&b, "hello", "world");
    BSON_APPEND_BOOL (&b, "bool", true);
    double t = tmr.elapsed();
    // std::string str = bson_as_json (&b, NULL);
    // std::cout << "BSON Basic (ws: " << b.len << ")("<< t<< "): " << str << std::endl;
    cout_experiments("BSON Basic", b.len, t);
  }


  {
    tmr.reset();
    rapidjson::Document r(rapidjson::kObjectType);
    auto &alloc = r.GetAllocator();
    r.AddMember("a", 1, alloc);
    r.AddMember("hello", "world", alloc);
    r.AddMember("bool", true, alloc);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    r.Accept(writer);
    std::string x = buffer.GetString();
    double t = tmr.elapsed();
    // std::cout << "RapidJSON basic (ws: "<< x.length() <<")("<< t<< "): " << x << std::endl;
    cout_experiments("RapidJSON basic", x.length(), t);
  }

  {
    tmr.reset();
    bson_t b = BSON_INITIALIZER;

    BSON_APPEND_UTF8 (&b, "hello", "world");
    bson_append_binary(&b, "binary_data", -1, BSON_SUBTYPE_BINARY, (uint8_t* ) fakedata, fakedata_size);
    double t = tmr.elapsed();
    std::string str = bson_as_json (&b, NULL);
    // std::cout << "BSON Binary "<< std::setfill(' ') << std::setw(30) << "(ws: " << b.len << ")("<< t<< "): " << std::left << std::endl;
    cout_experiments("BSON Binary", b.len, t);
  }

  {
    tmr.reset();
    rapidjson::Document r(rapidjson::kObjectType);
    auto &alloc = r.GetAllocator();
    r.AddMember("hello", "world", alloc);
    r.AddMember("binary_data", "", alloc);
    r["binary_data"].SetString(fakedata, fakedata_size, alloc);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    r.Accept(writer);
    std::string x = buffer.GetString();
    double t = tmr.elapsed();
    // std::cout << "RapidJson 'Binary' (ws: "<< x.length() <<")("<< t<< "): " << std::endl;
    cout_experiments("RapidJson Binary", x.length(), t);
  }

  {
    tmr.reset();
    rapidjson::Document r(rapidjson::kObjectType);
    auto &alloc = r.GetAllocator();
    r.AddMember("hello", "world", alloc);
    std::string base64_data = base64_encode(((const BYTE*) &fakedata), fakedata_size);
    r.AddMember("binary_data", base64_data, alloc);
    // r["binary_data"].SetString(fakedata, fakedata_size, alloc);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    r.Accept(writer);
    std::string x = buffer.GetString();
    double t = tmr.elapsed();
    // std::cout << "RapidJson + base64_encode(binary) (ws: "<< x.length() <<")("<< t<< "): " << std::endl;
    cout_experiments("RapidJson + base64_encode(binary)", x.length(), t);
  }

  // // Access the actual bson encoded data
  // {
  //   tmr.reset();
  //   bson_t b = BSON_INITIALIZER;

  //   BSON_APPEND_UTF8 (&b, "hello", "world");
  //   double t = tmr.elapsed();
  //   std::string str = bson_as_json (&b, NULL);
  //   const uint8_t *bson_data = bson_get_data (&b);
  //   uint32_t bson_size = b.len;
  //   for(uint32_t i = 0; i < bson_size; i++){
  //     std::cout << "0x" << std::hex  << std::setfill('0') << std::setw(2) << ((int)bson_data[i]); 
  //     std::cout << " ";
  //   }
  //   std::cout << std::endl;
  //   cout_experiments("B hello world access example", bson_size, t);
  //   // std::cout << "B hello world access example (size: " << bson_size <<") ("<< t<< "): " << str << std::endl;
  // }


  return i;
}
