#ifndef GBAXX_SIZED_BOOL_HPP
#define GBAXX_SIZED_BOOL_HPP

#include <array>
#include <limits>

#include <gba/int_type.hpp>
#include <gba/math.hpp>

namespace gba {

template <unsigned int Size>
class [[gnu::packed]] sized_bool_padding {
protected:
	constexpr sized_bool_padding() {}
private:
	char	m_padding[Size];
};

class [[gnu::packed]] sized_bool_no_padding {
protected:
	constexpr sized_bool_no_padding() {}
};

// TODO : Make volatile compatible
template <unsigned int Size>
class [[gnu::packed]] sized_bool : std::conditional<Size != math::floor2( Size ), sized_bool_padding<Size - math::floor2( Size )>, sized_bool_no_padding>::type {
	static_assert( Size > 0, "sized_bool Size cannot be zero" );
	static_assert( Size < 16, "sized_bool Size must be less than 16" );
public:
	using value_type = typename uint_sized_type<math::floor2( Size )>::type;

	constexpr sized_bool() {}

	constexpr sized_bool( value_type value ) {
		operator=( value );
	}

	constexpr sized_bool& operator=( bool value ) {
		m_data = value;
		return *this;
	}

	constexpr operator value_type() const {
		return m_data;
	}

	constexpr sized_bool& operator+=( value_type rhs ) {
		operator=( *this + rhs );
		return *this;
	}

	constexpr sized_bool& operator-=( value_type rhs ) {
		operator=( *this - rhs );
		return *this;
	}

	constexpr sized_bool& operator*=( value_type rhs ) {
		operator=( *this * rhs );
		return *this;
	}

	constexpr sized_bool& operator/=( value_type rhs ) {
		operator=( *this / rhs );
		return *this;
	}

	constexpr sized_bool& operator%=( value_type rhs ) {
		operator=( *this % rhs );
		return *this;
	}

	constexpr sized_bool& operator>>=( value_type rhs ) {
		operator=( *this >> rhs );
		return *this;
	}

	constexpr sized_bool& operator<<=( value_type rhs ) {
		operator=( *this << rhs );
		return *this;
	}

	constexpr sized_bool& operator|=( value_type rhs ) {
		operator=( *this | rhs );
		return *this;
	}

	constexpr sized_bool& operator&=( value_type rhs ) {
		operator=( *this & rhs );
		return *this;
	}

	constexpr sized_bool& operator^=( value_type rhs ) {
		operator=( *this ^ rhs );
		return *this;
	}

	constexpr sized_bool& operator|=( const sized_bool& rhs ) {
		m_data |= rhs.m_data;
		return *this;
	}

	constexpr sized_bool& operator&=( const sized_bool& rhs ) {
		m_data &= rhs.m_data;
		return *this;
	}

	constexpr sized_bool& operator^=( const sized_bool& rhs ) {
		m_data ^= rhs.m_data;
		return *this;
	}

private:
	value_type		m_data;

};

} // gba

template <unsigned int Size>
struct std::numeric_limits<gba::sized_bool<Size>> : public std::numeric_limits<bool> {};

template <unsigned int Size>
struct std::is_integral<gba::sized_bool<Size>> : std::is_integral<bool> {};

template <unsigned int Size>
struct std::is_arithmetic<gba::sized_bool<Size>> : std::is_arithmetic<bool> {};

#endif // define GBAXX_SIZED_BOOL_HPP