#ifndef DCOSTRMPP_H
#define DCOSTRMPP_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcostrma.h"
#include <vector>

class DCMTK_DCMDATA_EXPORT DcmVectorConsumer: public DcmConsumer
{
  friend class DcmVectorStream;
public:
  DcmVectorConsumer();
  virtual ~DcmVectorConsumer();
  virtual OFBool good() const;
  virtual OFCondition status() const;
  virtual OFBool isFlushed() const;
  virtual offile_off_t avail() const;
  virtual offile_off_t write(const void *buf, offile_off_t buflen);
  virtual void flush();

private:
  DcmVectorConsumer(const DcmVectorConsumer&);
  DcmVectorConsumer& operator=(const DcmVectorConsumer&);
  virtual std::vector<char>& getVector();
  OFCondition status_;
  std::vector<char> v_;
};


class DCMTK_DCMDATA_EXPORT DcmVectorStream: public DcmOutputStream
{
public:
  DcmVectorStream();
  virtual ~DcmVectorStream();
  const std::vector<char>& getVector();
private:
  DcmVectorStream(const DcmVectorStream&);
  DcmVectorStream& operator=(const DcmVectorStream&);
  DcmVectorConsumer consumer_;
};

#endif
