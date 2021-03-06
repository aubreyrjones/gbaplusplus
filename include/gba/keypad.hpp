#ifndef GBAXX_KEYPAD_HPP
#define GBAXX_KEYPAD_HPP

#include <cstring>

#include <gba/int.hpp>
#include <gba/int_type.hpp>

namespace gba {
namespace keypad {

class [[gnu::packed]] raw_input {
public:
	uint16 value() const {
		// Flip bits when reading so 1 means pressed
		// Flipping is 1 instruction, vs XOR's 2 instructions
		return ~m_bits;
	}

private:
	uint16	m_bits;

};

struct [[gnu::packed]] irq_control {

	bool	button_a : 1,
			button_b : 1,
			select : 1,
			start : 1,
			dpad_right : 1,
			dpad_left : 1,
			dpad_up : 1,
			dpad_down : 1,
			button_r : 1,
			button_l : 1,
			: 4;

	bool	irq_emit : 1,
			irq_condition_and : 1;

};

static constexpr auto Select = ( 0x1 << 2 );
static constexpr auto Start = ( 0x1 << 3 );

namespace button {

static constexpr auto A = ( 0x1 << 0 );
static constexpr auto B = ( 0x1 << 1 );
static constexpr auto R = ( 0x1 << 8 );
static constexpr auto L = ( 0x1 << 9 );

} // button

namespace dpad {

static constexpr auto Right = ( 0x1 << 4 );
static constexpr auto Left = ( 0x1 << 5 );
static constexpr auto Up = ( 0x1 << 6 );
static constexpr auto Down = ( 0x1 << 7 );

} // dpad

class [[gnu::packed]] state {
public:

	constexpr state() : m_bitmask {} {}
	constexpr state( uint16 s ) : m_bitmask( s ) {}
	constexpr state( const state& other ) : m_bitmask( other.m_bitmask ) {}

	state( const raw_input& raw ) : m_bitmask( raw.value() ) {}

	template <typename Type = int_sized_type<1>::fast>
	constexpr Type axis_x() const {
		Type axis = 0;
		if ( m_bitmask & dpad::Right ) axis += 1;
		if ( m_bitmask & dpad::Left ) axis -= 1;
		return axis;
	}

	template <typename Type = int_sized_type<1>::fast>
	constexpr Type axis_y() const {
		Type axis = 0;
		if ( m_bitmask & dpad::Up ) axis += 1;
		if ( m_bitmask & dpad::Down ) axis -= 1;
		return axis;
	}

	constexpr bool button_a() const {
		return m_bitmask & button::A;
	}

	constexpr bool button_b() const {
		return m_bitmask & button::B;
	}

	constexpr bool select() const {
		return m_bitmask & Select;
	}

	constexpr bool start() const {
		return m_bitmask & Start;
	}

	constexpr bool dpad_right() const {
		return m_bitmask & dpad::Right;
	}

	constexpr bool dpad_left() const {
		return m_bitmask & dpad::Left;
	}

	constexpr bool dpad_up() const {
		return m_bitmask & dpad::Up;
	}

	constexpr bool dpad_down() const {
		return m_bitmask & dpad::Down;
	}

	constexpr bool button_r() const {
		return m_bitmask & button::R;
	}

	constexpr bool button_l() const {
		return m_bitmask & button::L;
	}

private:
	uint16	m_bitmask;

};

class events {
public:
	constexpr events() : m_bitmask( 0 ), m_diff( 0 ) {}

	void update( const raw_input& raw ) {
		m_diff = m_bitmask ^ raw.value(); // What keys have changed state?
		if ( m_diff ) {
			m_bitmask = raw.value(); // What keys are pressed?
		}
	}

	constexpr bool key_pressed( uint16 k ) const {
		return ( m_bitmask & k ) == k;
	}

	constexpr bool key_released( uint16 k ) const {
		return ( m_bitmask & k ) == 0;
	}

	constexpr bool key_down( uint16 k ) const {
		if ( ( m_diff & k ) == k ) {
			return ( m_bitmask & k ) == k;
		}
		return false;
	}

	constexpr bool key_up( uint16 k ) const {
		if ( ( m_diff & k ) == k ) {
			return ( m_bitmask & k ) == 0;
		}
		return false;
	}

	template <typename Type>
	constexpr bool axis_changed( Type& mask ) const {
		if ( m_diff & 0xf0 ) {
			mask = ( m_bitmask >> 4 ) & 0xf;
			return true;
		}
		return false;
	}

private:
	uint16	m_bitmask;
	uint16	m_diff;
	
};

} // keypad
} // gba

#endif // define GBAXX_KEYPAD_HPP
