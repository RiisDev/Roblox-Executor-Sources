#include "animator.hpp"

#include <unordered_map>
#include <cmath>

#ifndef PI
#define PI 3.1415926545
#endif

static std::unordered_map<std::string, menu::animator_t*> animators;

menu::animator_t::animator_t( )
{}

void menu::animator_t::reset( )
{
    time_i = managers::renderer_mgr.epoch( );
}

void menu::animator_t::timeout( float seconds )
{
    time_i = managers::renderer_mgr.epoch( ) + seconds;
}

menu::animator_t& menu::animator_t::get( const std::string& id )
{
    animator_t* res = nullptr;
    if ( animators.find( id ) == animators.end( ) ) {
        res = new animator_t;
        animators.emplace( id, res );
    }
    else res = animators[ id ];
    return *res;
}

void menu::animator_t::dispose( )
{
    delete this;
}

/* easings */
float menu::easings::linear( float t )
{
    return t;
}

float menu::easings::in_sine( float t )
{
    return sin( 1.5707963 * t );
}

float menu::easings::out_sine( float t )
{
    return 1 + sin( 1.5707963 * ( --t ) );
}

float menu::easings::inout_sine( float t )
{
    return 0.5 * ( 1 + sin( 3.1415926 * ( t - 0.5 ) ) );
}

float menu::easings::in_quad( float t )
{
    return t * t;
}

float menu::easings::out_quad( float t )
{
    return t * ( 2 - t );
}

float menu::easings::inout_quad( float t )
{
    return t < 0.5 ? 2 * t * t : t * ( 4 - 2 * t ) - 1;
}

float menu::easings::in_cubic( float t )
{
    return t * t * t;
}

float menu::easings::out_cubic( float t )
{
    return 1 + ( --t ) * t * t;
}

float menu::easings::inout_cubic( float t )
{
    return t < 0.5 ? 4 * t * t * t : 1 + ( --t ) * ( 2 * ( --t ) ) * ( 2 * t );
}

float menu::easings::in_quart( float t )
{
    t *= t;
    return t * t;
}

float menu::easings::out_quart( float t )
{
    t = ( --t ) * t;
    return 1 - t * t;
}

float menu::easings::inout_quart( float t )
{
    if ( t < 0.5 ) {
        t *= t;
        return 8 * t * t;
    }
    else {
        t = ( --t ) * t;
        return 1 - 8 * t * t;
    }
}

float menu::easings::in_quint( float t )
{
    float t2 = t * t;
    return t * t2 * t2;
}

float menu::easings::out_quint( float t )
{
    float t2 = ( --t ) * t;
    return 1 + t * t2 * t2;
}

float menu::easings::inout_quint( float t )
{
    float t2;
    if ( t < 0.5 ) {
        t2 = t * t;
        return 16 * t * t2 * t2;
    }
    else {
        t2 = ( --t ) * t;
        return 1 + 16 * t * t2 * t2;
    }
}

float menu::easings::in_expo( float t )
{
    return ( pow( 2, 8 * t ) - 1 ) / 255;
}

float menu::easings::out_expo( float t )
{
    return 1 - pow( 2, -8 * t );
}

float menu::easings::inout_expo( float t )
{
    if ( t < 0.5 ) {
        return ( pow( 2, 16 * t ) - 1 ) / 510;
    }
    else {
        return 1 - 0.5 * pow( 2, -16 * ( t - 0.5 ) );
    }
}

float menu::easings::in_circ( float t )
{
    return 1 - sqrt( 1 - t );
}

float menu::easings::out_circ( float t )
{
    return sqrt( t );
}

float menu::easings::inout_circ( float t )
{
    if ( t < 0.5 ) {
        return ( 1 - sqrt( 1 - 2 * t ) ) * 0.5;
    }
    else {
        return ( 1 + sqrt( 2 * t - 1 ) ) * 0.5;
    }
}

float menu::easings::in_back( float t )
{
    return t * t * ( 2.70158 * t - 1.70158 );
}

float menu::easings::out_back( float t )
{
    return 1 + ( --t ) * t * ( 2.70158 * t + 1.70158 );
}

float menu::easings::inout_back( float t )
{
    if ( t < 0.5 ) {
        return t * t * ( 7 * t - 2.5 ) * 2;
    }
    else {
        return 1 + ( --t ) * t * 2 * ( 7 * t + 2.5 );
    }
}

float menu::easings::in_elastic( float t )
{
    float t2 = t * t;
    return t2 * t2 * sin( t * PI * 4.5 );
}

float menu::easings::out_elastic( float t )
{
    float t2 = ( t - 1 ) * ( t - 1 );
    return 1 - t2 * t2 * cos( t * PI * 4.5 );
}

float menu::easings::inout_elastic( float t )
{
    float t2;
    if ( t < 0.45 ) {
        t2 = t * t;
        return 8 * t2 * t2 * sin( t * PI * 9 );
    }
    else if ( t < 0.55 ) {
        return 0.5 + 0.75 * sin( t * PI * 4 );
    }
    else {
        t2 = ( t - 1 ) * ( t - 1 );
        return 1 - 8 * t2 * t2 * sin( t * PI * 9 );
    }
}

float menu::easings::in_bounce( float t )
{
    return pow( 2, 6 * ( t - 1 ) ) * abs( sin( t * PI * 3.5 ) );
}

float menu::easings::out_bounce( float t )
{
    return 1 - pow( 2, -6 * t ) * abs( cos( t * PI * 3.5 ) );
}

float menu::easings::inout_bounce( float t )
{
    if ( t < 0.5 ) {
        return 8 * pow( 2, 8 * ( t - 1 ) ) * abs( sin( t * PI * 7 ) );
    }
    else {
        return 1 - 8 * pow( 2, -8 * t ) * abs( sin( t * PI * 7 ) );
    }
}
