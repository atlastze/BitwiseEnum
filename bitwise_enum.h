// Copyright (c) 2016, Kylin Sage <kylinsage@gmail.com>
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef BITWISE_ENUM_H
#define BITWISE_ENUM_H

#include<type_traits>

// General traits class template
template<typename E>
struct enable_bitwise_operators{
	static const bool enable=false;
};

// Partial template specialization for enumerations 
// to be enabled with bitwise operators.
#define ENABLE_BITWISE_OPERATORS(enumeration) \
template<> \
struct enable_bitwise_operators<enumeration> { \
	static const bool enable=true; \
};

// Bitwise OR
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E>::type
operator|(E lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	return static_cast<E>(
		static_cast<underlying_type_t>(lhs) | static_cast<underlying_type_t>(rhs));
}

// Bitwise AND
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E>::type
operator&(E lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	return static_cast<E>(
		static_cast<underlying_type_t>(lhs) & static_cast<underlying_type_t>(rhs));
}

// Bitwise XOR
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E>::type
operator^(E lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	return static_cast<E>(
		static_cast<underlying_type_t>(lhs) ^ static_cast<underlying_type_t>(rhs));
}

// Bitwise NOT
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E>::type
operator~(E lhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	return static_cast<E>(
		~static_cast<underlying_type_t>(lhs));
}

// Bitwise OR - compound assignment
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E&>::type
operator|=(E& lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	lhs=static_cast<E>(
		static_cast<underlying_type_t>(lhs) | static_cast<underlying_type_t>(rhs));
	return lhs;
}

// Bitwise AND - compound assignment
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E&>::type
operator&=(E& lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	lhs=static_cast<E>(
		static_cast<underlying_type_t>(lhs) & static_cast<underlying_type_t>(rhs));
	return lhs;
}

// Bitwise XOR - compound assignment
template<typename E>
typename std::enable_if<enable_bitwise_operators<E>::enable,E&>::type
operator^=(E& lhs,E rhs){
	using underlying_type_t = typename std::underlying_type<E>::type;
	lhs=static_cast<E>(
		static_cast<underlying_type_t>(lhs) ^ static_cast<underlying_type_t>(rhs));
	return lhs;
}

#endif /* BITWISE_ENUM_H */
