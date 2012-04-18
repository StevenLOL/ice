// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_PROTOCOL_H
#define ICE_PROTOCOL_H

#include <Ice/Config.h>
#include <Ice/Version.h>

namespace IceInternal
{

//
// Size of the Ice protocol header
//
// Magic number (4 Bytes)
// Protocol version major (Byte)
// Protocol version minor (Byte)
// Encoding version major (Byte)
// Encoding version minor (Byte)
// Message type (Byte)
// Compression status (Byte)
// Message size (Int)
//
const ::Ice::Int headerSize = 14;

//
// The magic number at the front of each message
//
extern const ::Ice::Byte magic[4];

//
// The current Ice protocol, protocol encoding and encoding version
//
const ::Ice::Byte protocolMajor = 1;
const ::Ice::Byte protocolMinor = 0;
const ::Ice::Byte protocolEncodingMajor = 1;
const ::Ice::Byte protocolEncodingMinor = 0;

const ::Ice::Byte encodingMajor = 1;
const ::Ice::Byte encodingMinor = 1;

//
// The Ice protocol message types
//
const ::Ice::Byte requestMsg = 0;
const ::Ice::Byte requestBatchMsg = 1;
const ::Ice::Byte replyMsg = 2;
const ::Ice::Byte validateConnectionMsg = 3;
const ::Ice::Byte closeConnectionMsg = 4;

//
// The request header, batch request header and reply header.
//
extern const ::Ice::Byte requestHdr[headerSize + sizeof(Ice::Int)];
extern const ::Ice::Byte requestBatchHdr[headerSize + sizeof(Ice::Int)];
extern const ::Ice::Byte replyHdr[headerSize];

//
// IPv4/IPv6 support enumeration.
//
enum ProtocolSupport
{
    EnableIPv4,
    EnableIPv6,
    EnableBoth
};

// Forward declaration
class BasicStream;

void stringToMajorMinor(const ::std::string&, Ice::Byte&, Ice::Byte&);

template<typename T> std::string
versionToString(const T& v)
{
    std::ostringstream os;
    os << v;
    return os.str();
}

template<typename T> T
stringToVersion(const ::std::string& str)
{
    T v;
    stringToMajorMinor(str, v.major, v.minor);
    return v;
}

template<typename T> bool
isSupported(const T& version, const T& supported)
{
    return version.major == supported.major && version.minor <= supported.minor;
}

void throwUnsupportedProtocolException(const char*, int, const Ice::ProtocolVersion&, const Ice::ProtocolVersion&);
void throwUnsupportedEncodingException(const char*, int, const Ice::EncodingVersion&, const Ice::EncodingVersion&);

}

namespace Ice
{

extern const ProtocolVersion Protocol_1_0;

extern const EncodingVersion Encoding_1_0;
extern const EncodingVersion Encoding_1_1;

extern const ProtocolVersion currentProtocol;
extern const EncodingVersion currentProtocolEncoding;

extern const EncodingVersion currentEncoding;

inline ::std::string 
protocolVersionToString(const Ice::ProtocolVersion& v)
{
    return IceInternal::versionToString<ProtocolVersion>(v);
}

inline ::Ice::ProtocolVersion 
stringToProtocolVersion(const ::std::string& v)
{
    return IceInternal::stringToVersion<ProtocolVersion>(v);
}

inline ::std::string 
encodingVersionToString(const Ice::EncodingVersion& v)
{
    return IceInternal::versionToString<EncodingVersion>(v);
}

inline ::Ice::EncodingVersion 
stringToEncodingVersion(const ::std::string& v)
{
    return IceInternal::stringToVersion<EncodingVersion>(v);
}

inline std::ostream&
operator<<(std::ostream& out, const ProtocolVersion& version)
{
    return out << static_cast<int>(version.major) << "." << static_cast<int>(version.minor);
}

inline std::ostream&
operator<<(std::ostream& out, const EncodingVersion& version)
{
    return out << static_cast<int>(version.major) << "." << static_cast<int>(version.minor);
}

inline void
checkSupportedProtocol(const ProtocolVersion& v)
{
    if(!IceInternal::isSupported(v, currentProtocol))
    {
        IceInternal::throwUnsupportedProtocolException(__FILE__, __LINE__, v, currentProtocol);
    }
}

inline void
checkSupportedProtocolEncoding(const EncodingVersion& v)
{
    if(!IceInternal::isSupported(v, currentProtocolEncoding))
    {
        IceInternal::throwUnsupportedEncodingException(__FILE__, __LINE__, v, currentProtocolEncoding);
    }
}

inline void
checkSupportedEncoding(const EncodingVersion& v)
{
    if(!IceInternal::isSupported(v, currentEncoding))
    {
        IceInternal::throwUnsupportedEncodingException(__FILE__, __LINE__, v, currentEncoding);
    }
}

}

#endif
