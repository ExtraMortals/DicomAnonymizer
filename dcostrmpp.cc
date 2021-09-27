/*
 *
 *  Copyright (C) 2002-2021, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmdata
 *
 *  Author:  Jasper den Otter, Marco Eichelberg
 *
 *  Purpose: DcmVectorStream and related classes,
 *    implements streamed output to stdout.
 *
 */

#include "dcmtk/config/osconfig.h"
#include "dcostrmpp.h"
#include "dcmtk/dcmdata/dcerror.h"
#include "dcmtk/ofstd/ofconsol.h"

DcmVectorConsumer::DcmVectorConsumer()
: DcmConsumer()
, status_(EC_Normal)
, v_()
{
  v_.reserve(524288000ul);
}

DcmVectorConsumer::~DcmVectorConsumer()
{
}

OFBool DcmVectorConsumer::good() const
{
  return status_.good();
}

OFCondition DcmVectorConsumer::status() const
{
  return status_;
}

OFBool DcmVectorConsumer::isFlushed() const
{
  return OFTrue;
}

offile_off_t DcmVectorConsumer::avail() const
{
  #undef max
  return OFstatic_cast(offile_off_t, std::numeric_limits<offile_off_t>::max());
}

offile_off_t DcmVectorConsumer::write(const void *buf, offile_off_t buflen)
{
  for (auto i = 0 ; i < buflen ; i++){
    v_.emplace_back(OFstatic_cast(const char*, buf)[i]);
  }
  return OFstatic_cast(offile_off_t, buflen);
}

void DcmVectorConsumer::flush()
{
}

std::vector<char>& DcmVectorConsumer::getVector()
{
  return v_;
}

/* ======================================================================= */

DcmVectorStream::DcmVectorStream()
: DcmOutputStream(&consumer_) // safe because DcmVectorStream only stores pointer
, consumer_()
{

}



DcmVectorStream::~DcmVectorStream()
{
}

const std::vector<char>& DcmVectorStream::getVector()
{
  return consumer_.getVector();
}
