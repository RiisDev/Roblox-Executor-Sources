#include "adapters.hpp"

#include <format>

#include <cpr/cpr.h>

#include <WinDNS.h>
#include <winsock.h>

#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Dnsapi.lib")

std::vector< fingerprinting::network_adapter_t > fingerprinting::adapters_t::get_adapters( )
{
    std::vector< network_adapter_t > result;

    ULONG buffer_size = sizeof( IP_ADAPTER_INFO );
    auto adapter_info = new char[ buffer_size ];

    if ( GetAdaptersInfo( reinterpret_cast< IP_ADAPTER_INFO* >( adapter_info ), &buffer_size ) == ERROR_BUFFER_OVERFLOW )
    {
        delete[ ] adapter_info;
        adapter_info = new char[ buffer_size ];
    }

    if ( const auto value = GetAdaptersInfo( reinterpret_cast< IP_ADAPTER_INFO* >( adapter_info ), &buffer_size ); value == NO_ERROR )
    {
        const auto* ptr = reinterpret_cast< IP_ADAPTER_INFO* >( adapter_info );

        while ( ptr )
        {
            result.push_back( make_adapter( ptr ) );
            ptr = ptr->Next;
        }
    }

    if ( adapter_info )
        delete[ ] adapter_info;

    return result;
}

fingerprinting::network_adapter_t fingerprinting::adapters_t::make_adapter( const IP_ADAPTER_INFO* adapter )
{
    network_adapter_t result;

    result.name = adapter->AdapterName;
    result.desc = adapter->Description;

    result.gateway = adapter->GatewayList.IpAddress.String;
    result.ip_address = adapter->IpAddressList.IpAddress.String;

    switch ( adapter->Type )
    {
    case MIB_IF_TYPE_OTHER:
        result.type = "OTHER";
        break;

    case MIB_IF_TYPE_ETHERNET:
        result.type = "ETHERNET";
        break;

    case MIB_IF_TYPE_PPP:
        result.type = "PPP";
        break;

    case MIB_IF_TYPE_LOOPBACK:
        result.type = "LOOPBACK";
        break;

    case MIB_IF_TYPE_SLIP:
        result.type = "ATM";
        break;

    case IF_TYPE_IEEE80211:
        result.type = "WIFI";
        break;

    default:
        result.type = "UNKNOWN";
    }

    if ( adapter->AddressLength == 6 )
        result.mac_address = std::format
        (
            "{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}",
            adapter->Address[ 0 ], adapter->Address[ 1 ],
            adapter->Address[ 2 ], adapter->Address[ 3 ],
            adapter->Address[ 4 ], adapter->Address[ 5 ]
        );

    return result;
}

fingerprinting::json_t fingerprinting::adapters_t::serialize_adapter( const network_adapter_t adapter )
{
    json_t object;

    object[ "name" ] = adapter.name;
    object[ "description" ] = adapter.desc;
    object[ "gateway" ] = adapter.gateway;
    object[ "ip_address" ] = adapter.ip_address;
    object[ "type" ] = adapter.type;

    auto& mac = object[ "mac_address" ];

    if ( !adapter.mac_address.empty( ) )
        mac = adapter.mac_address;

    return object;
}

void fingerprinting::adapters_t::serialize_ip( )
{
    WSADATA data;
    char host[ 255 ];
    WSAStartup( MAKEWORD( 2, 2 ), &data );
    gethostname( host, 256 );

#ifndef _DEBUG
    auto& ip = _data[ "ip_address" ];

    auto result = cpr::Get( cpr::Url { "https://api.ipify.org/" } );
#endif
    _data[ "hostname" ] = host;
    
#ifndef _DEBUG
    if ( result.status_code == 200 )
        ip = result.text;
#endif

    WSACleanup( );
}

void fingerprinting::adapters_t::serialize_all( )
{
    json_t array;

    for ( const auto& adapter : get_adapters( ) )
    {
        array.push_back( serialize_adapter( adapter ) );
    }

    _data[ "network_adapters" ] = array;

    serialize_ip( );
}

fingerprinting::adapters_t::adapters_t( )
{
    serialize_all( );

    _object[ "network" ] = _data;
}
