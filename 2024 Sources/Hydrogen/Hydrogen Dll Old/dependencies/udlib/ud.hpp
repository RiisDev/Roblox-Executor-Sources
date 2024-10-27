#pragma once
#include <optional>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <string_view>
#include <fstream>
#include <unordered_map>

#include <Windows.h>
#include <winternl.h>

#if defined(_MSC_VER)
#define UD_FORCEINLINE __forceinline
#pragma warning( push )
#pragma warning( disable : 4244 4083 )
#else
#define UD_FORCEINLINE __attribute__( ( always_inline ) )
#endif

#define ud_encode_c( str ) ud::rot::decode( ud::rot::rot_t<str>{ } ).data
#define ud_encode( str ) std::string_view( ud::rot::decode( ud::rot::rot_t<str>{ } ) )

#define ud_xorstr_c( str ) ud::xorstr::decrypt( ud::xorstr::xorstr_t< str, __COUNTER__ + 1 ^ 0x90 >{ } ).data
#define ud_xorstr( str ) std::string_view{ ud::xorstr::decrypt( ud::xorstr::xorstr_t< str, __COUNTER__ + 1 ^ 0x90 >{ } ) }

#define ud_stack_str( str ) ud::details::comp_string_t{ str }.data

#define ud_import( mod, func )	reinterpret_cast< decltype( &func ) >( ud::lazy_import::find_module_export< TEXT( mod ), #func >( ) )
#define ud_first_import( func ) reinterpret_cast< decltype( &func ) >( ud::lazy_import::find_first_export< #func >( ) )

// preprocessed settings due to MSVC (not clang or gcc) throwing errors even in `if constexpr` bodies
#define UD_USE_SEH false

namespace ud
{
    namespace details
    {
        struct LDR_DATA_TABLE_ENTRY32
        {
            LIST_ENTRY in_load_order_links;

            std::uint8_t pad [ 16 ];
            std::uintptr_t dll_base;
            std::uintptr_t entry_point;
            std::size_t size_of_image;

            UNICODE_STRING full_name;
            UNICODE_STRING base_name;
        };

        struct LDR_DATA_TABLE_ENTRY64
        {
            LIST_ENTRY in_load_order_links;
            LIST_ENTRY dummy_0;
            LIST_ENTRY dummy_1;

            std::uintptr_t dll_base;
            std::uintptr_t entry_point;
            union {
                unsigned long size_of_image;
                const char* _dummy;
            };

            UNICODE_STRING full_name;
            UNICODE_STRING base_name;
        };

#if defined( _M_X64 )
        using LDR_DATA_TABLE_ENTRY = LDR_DATA_TABLE_ENTRY64;
#else
        using LDR_DATA_TABLE_ENTRY = LDR_DATA_TABLE_ENTRY32;
#endif

        template < std::size_t sz >
        struct comp_string_t
        {
            std::size_t size = sz;
            char data [ sz ] { };

            comp_string_t( ) = default;
            consteval explicit comp_string_t( const char( &str ) [ sz ] )
            {
                std::copy_n( str, sz, data );
            }

            constexpr explicit operator std::string_view( ) const
            {
                return { data, size };
            }
        };

        template < std::size_t sz >
        struct wcomp_string_t
        {
            std::size_t size = sz;
            wchar_t data [ sz ] { };

            wcomp_string_t( ) = default;
            consteval explicit wcomp_string_t( const wchar_t( &str ) [ sz ] )
            {
                std::copy_n( str, sz, data );
            }

            constexpr explicit operator std::wstring_view( ) const
            {
                return { data, size };
            }
        };

        inline constexpr std::uint64_t multiplier = 0x5bd1e995;
        inline consteval std::uint64_t get_seed( )
        {
            constexpr auto time_str = __TIME__;
            constexpr auto time_len = sizeof( __TIME__ ) - 1;

            constexpr auto time_int = [ ] ( const char* const str, const std::size_t len )
            {
                auto res = 0ull;
                for ( auto i = 0u; i < len; ++i )
                    if ( str [ i ] >= '0' && str [ i ] <= '9' )
                        res = res * 10 + str [ i ] - '0';

                return res;
            }( time_str, time_len );

            return time_int;
        }

        template < auto v >
        struct constant_t
        {
            enum : decltype( v )
            {
                value = v
            };
        };

        template < auto v >
        inline constexpr auto constant_v = constant_t< v >::value;

#undef max
#undef min

        template < std::uint32_t seq >
        consteval std::uint64_t recursive_random( )
        {
            constexpr auto seed = get_seed( );
            constexpr auto mask = std::numeric_limits< std::uint64_t >::max( );

            constexpr auto x = ( ( seq * multiplier ) + seed ) & mask;
            constexpr auto x_prime = ( x >> 0x10 ) | ( x << 0x10 );

            return constant_v< x_prime >;
        }
    }

    namespace rot
    {
        template < details::comp_string_t str >
        struct rot_t
        {
            char rotted [ str.size ];

            [[nodiscard]] consteval const char* encoded( ) const
            {
                return rotted;
            }

            consteval rot_t( )
            {
                for ( auto i = 0u; i < str.size; ++i )
                {
                    const auto c = str.data [ i ];
                    const auto set = c >= 'A' && c <= 'Z' ? 'A' : c >= 'a' && c <= 'z' ? 'a' : c;

                    if ( set == 'a' || set == 'A' )
                        rotted [ i ] = ( c - set - 13 + 26 ) % 26 + set;

                    else
                        rotted [ i ] = c;
                }
            }
        };

        template < details::comp_string_t str >
        UD_FORCEINLINE details::comp_string_t< str.size > decode( rot_t< str > encoded )
        {
            details::comp_string_t< str.size > result { };

            for ( auto i = 0u; i < str.size; ++i )
            {
                const auto c = encoded.rotted [ i ];
                const auto set = c >= 'A' && c <= 'Z' ? 'A' : c >= 'a' && c <= 'z' ? 'a' : c;

                if ( set == 'a' || set == 'A' )
                    result.data [ i ] = ( c - set - 13 + 26 ) % 26 + set;

                else
                    result.data [ i ] = c;
            }

            return result;
        }
    }

    namespace fnv
    {
        inline constexpr std::uint32_t fnv_1a( const char* const str, const std::size_t size )
        {
            constexpr auto prime = 16777619u;

            std::uint32_t hash = 2166136261;

            for ( auto i = 0u; i < size; ++i )
            {
                hash ^= str [ i ];
                hash *= prime;
            }

            return hash;
        }

        inline constexpr std::uint32_t fnv_1a( const wchar_t* const str, const std::size_t size )
        {
            constexpr auto prime = 16777619u;

            std::uint32_t hash = 2166136261;

            for ( auto i = 0u; i < size; ++i )
            {
                hash ^= static_cast< char >( str [ i ] );
                hash *= prime;
            }

            return hash;
        }

        inline constexpr std::uint32_t fnv_1a( const std::wstring_view str )
        {
            return fnv_1a( str.data( ), str.size( ) );
        }

        inline constexpr std::uint32_t fnv_1a( const std::string_view str )
        {
            return fnv_1a( str.data( ), str.size( ) );
        }

        template < details::comp_string_t str >
        consteval std::uint32_t fnv_1a( )
        {
            return fnv_1a( str.data, str.size );
        }

        template < details::wcomp_string_t str >
        consteval std::uint32_t fnv_1a( )
        {
            return fnv_1a( str.data, str.size );
        }
    }

    namespace xorstr
    {
        template < details::comp_string_t str, std::uint32_t key_multiplier >
        struct xorstr_t
        {
            char xored [ str.size ];

            [[nodiscard]] consteval std::uint64_t xor_key( ) const
            {
                return details::recursive_random< key_multiplier >( );
            }

            consteval xorstr_t( )
            {
                for ( auto i = 0u; i < str.size; ++i )
                    xored [ i ] = str.data [ i ] ^ xor_key( );
            }
        };

        template < details::comp_string_t str, std::uint32_t key_multiplier >
        UD_FORCEINLINE details::comp_string_t< str.size > decrypt( xorstr_t< str, key_multiplier > enc )
        {
            details::comp_string_t< str.size > result { };

            for ( auto i = 0u; i < str.size; ++i )
            {
                const auto c = enc.xored [ i ];

                result.data [ i ] = c ^ enc.xor_key( );
            }

            return result;
        }
    }

    namespace lazy_import
    {
        UD_FORCEINLINE std::uintptr_t get_module_handle( const std::uint64_t hash )
        {
#if defined( _M_X64 )
            const auto peb = reinterpret_cast< const PEB* >( __readgsqword( 0x60 ) );
#else
            const auto peb = reinterpret_cast< const PEB* >( __readfsdword( 0x30 ) );
#endif

            const auto modules = reinterpret_cast< const LIST_ENTRY* >( peb->Ldr->InMemoryOrderModuleList.Flink );

            for ( auto i = modules->Flink; i != modules; i = i->Flink )
            {
                const auto entry = reinterpret_cast< const details::LDR_DATA_TABLE_ENTRY* >( i );

                const auto name = entry->base_name.Buffer;
                const auto len = entry->base_name.Length;

                if ( fnv::fnv_1a( static_cast< const wchar_t* >( name ), len ) == hash )
                    return entry->dll_base;
            }

            return 0;
        }

        UD_FORCEINLINE void* find_primitive_export( const std::uint64_t dll_hash, const std::uint64_t function_hash )
        {
            const auto module = get_module_handle( dll_hash );

            if ( !module )
                return nullptr;

            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( module );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( module + dos->e_lfanew );

            const auto exports = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( module + nt->OptionalHeader.DataDirectory [ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );

            const auto names = reinterpret_cast< const std::uint32_t* >( module + exports->AddressOfNames );
            const auto ordinals = reinterpret_cast< const std::uint16_t* >( module + exports->AddressOfNameOrdinals );
            const auto functions = reinterpret_cast< const std::uint32_t* >( module + exports->AddressOfFunctions );

            for ( auto i = 0u; i < exports->NumberOfNames; ++i )
            {
                const auto name = reinterpret_cast< const char* >( module + names [ i ] );
                std::size_t len = 0;

                for ( ; name [ len ]; ++len );

                if ( fnv::fnv_1a( name, len ) == function_hash )
                    return reinterpret_cast< void* >( module + functions [ ordinals [ i ] ] );
            }

            return nullptr;
        }

        template < details::wcomp_string_t dll_name, details::comp_string_t function_name >
        UD_FORCEINLINE void* find_module_export( )
        {
            return find_primitive_export( fnv::fnv_1a< dll_name >( ), fnv::fnv_1a< function_name >( ) );
        }

        template < details::comp_string_t function_name >
        UD_FORCEINLINE void* find_first_export( )
        {
            constexpr auto function_hash = fnv::fnv_1a< function_name >( );

#if defined( _M_X64 )
            const auto peb = reinterpret_cast< const PEB* >( __readgsqword( 0x60 ) );
#else
            const auto peb = reinterpret_cast< const PEB* >( __readfsdword( 0x30 ) );
#endif

            const auto modules = reinterpret_cast< const LIST_ENTRY* >( peb->Ldr->InMemoryOrderModuleList.Flink );


            for ( auto i = modules->Flink; i != modules; i = i->Flink )
            {
                const auto entry = reinterpret_cast< const details::LDR_DATA_TABLE_ENTRY* >( i );

                const auto name = entry->base_name.Buffer;
                std::size_t len = 0;

                if ( !name )
                    continue;

                for ( ; name [ len ]; ++len );

                if ( const auto exp = find_primitive_export( fnv::fnv_1a( name, len ), function_hash ) )
                    return exp;
            }

            return nullptr;
        }
    }

    template < typename ty = std::uintptr_t >
    std::optional< ty > find_pattern_primitive( const std::uintptr_t start, const std::uintptr_t end, const std::string_view pattern )
    {
        std::vector< std::pair< bool, std::uint8_t > > bytes;

        for ( auto it = pattern.begin( ); it != pattern.end( ); ++it )
        {
            if ( *it == ' ' )
                continue;

            else if ( *it == '?' )
            {
                if ( it + 1 < pattern.end( ) && *( it + 1 ) == '?' )
                {
                    bytes.push_back( { true, 0x00 } );
                    ++it;
                }

                else
                    bytes.push_back( { false, 0x00 } );
            }

            else
            {
                if ( it + 1 == pattern.end( ) )
                    break;

                const auto get_byte = [ ] ( const std::string& x ) -> std::uint8_t
                {
                    return static_cast< std::uint8_t >( std::stoul( x, nullptr, 16 ) );
                };

                bytes.emplace_back( false, get_byte( std::string( it - 1, ( ++it ) + 1 ) ) );
            }
        }

        for ( auto i = reinterpret_cast< const std::uint8_t* >( start ); i < reinterpret_cast< const std::uint8_t* >( end ); )
        {
            auto found = true;
            for ( const auto& [is_wildcard, byte] : bytes )
            {
                ++i;

                if ( is_wildcard )
                    continue;

                if ( *i != byte )
                {
                    found = false;
                    break;
                }
            }

            if ( found )
                return ty( i - bytes.size( ) + 1 );
        }

        return std::nullopt;
    }

    struct segment_t
    {
        std::string_view name = "";
        std::uintptr_t start { }, end { };
        std::size_t size { };

        template < typename ty = std::uintptr_t >
        std::optional< ty > find_pattern( const std::string_view pattern ) const
        {
            return find_pattern_primitive< ty >( start, end, pattern );
        }

        segment_t( const std::string_view segment_name )
        {
            init( GetModuleHandle( nullptr ), segment_name );
        }

        segment_t( const void* const module, const std::string_view segment_name )
        {
            init( module, segment_name );
        }

        segment_t( const void* const handle, const IMAGE_SECTION_HEADER* section )
        {
            init( handle, section );
        }

        explicit segment_t( ) = default;

    private:
        void init( const void* const handle, const IMAGE_SECTION_HEADER* section )
        {
            name = std::string_view( reinterpret_cast< const char* >( section->Name ), 8 );
            start = reinterpret_cast< std::uintptr_t >( handle ) + section->VirtualAddress;
            end = start + section->Misc.VirtualSize;
            size = section->Misc.VirtualSize;
        }

        void init( const void* const handle, const std::string_view segment_name )
        {
            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( handle );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( reinterpret_cast< const std::uint8_t* >( handle ) + dos->e_lfanew );

            const auto section = reinterpret_cast< const IMAGE_SECTION_HEADER* >( reinterpret_cast< const std::uint8_t* >( &nt->OptionalHeader ) + nt->FileHeader.SizeOfOptionalHeader );

            for ( auto i = 0u; i < nt->FileHeader.NumberOfSections; ++i )
            {
                if ( std::string_view( reinterpret_cast< const char* >( section [ i ].Name ), 8 ).find( segment_name ) != std::string_view::npos )
                {
                    start = reinterpret_cast< std::uintptr_t >( handle ) + section [ i ].VirtualAddress;
                    end = start + section [ i ].Misc.VirtualSize;
                    size = section [ i ].Misc.VirtualSize;
                    name = segment_name;
                    return;
                }
            }
        }
    };

#pragma code_seg( push, ".text" )
    template < auto... bytes>
    struct shellcode_t
    {
        static constexpr std::size_t size = sizeof...( bytes );
        __declspec( allocate( ".text" ) ) static constexpr std::uint8_t data [ ] { bytes... };
    };
#pragma code_seg( pop )

    template < typename ty, auto... bytes >
    constexpr ty make_shellcode( )
    {
        return reinterpret_cast< const ty >( &shellcode_t< bytes... >::data );
    }

    template < std::uint8_t... bytes >
    UD_FORCEINLINE constexpr void emit( )
    {
#if defined( __clang__ ) || defined( __GNUC__ )
        constexpr std::uint8_t data [ ] { bytes... };

        for ( auto i = 0u; i < sizeof...( bytes ); ++i )
            __asm volatile( ".byte %c0\t\n" :: "i" ( data [ i ] ) );
#endif
    }

    template < std::size_t size, std::uint32_t seed = __COUNTER__ + 0x69, std::size_t count = 0 >
    UD_FORCEINLINE constexpr void emit_random( )
    {
        if constexpr ( count < size )
        {
            constexpr auto random = details::recursive_random< seed >( );
            emit< static_cast< std::uint8_t >( random ) >( );
            emit_random< size, static_cast< std::uint32_t >( random )* seed, count + 1 >( );
        }
    }

    inline bool is_valid_page( const void* const data, const std::uint32_t flags = PAGE_READWRITE )
    {
        MEMORY_BASIC_INFORMATION mbi { };

        if ( !VirtualQuery( data, &mbi, sizeof( mbi ) ) )
            return false;

        return mbi.Protect & flags;
    }

    struct export_t
    {
        std::string_view name;
        std::uint16_t ordinal { };
        std::uintptr_t address { };
    };

    struct module_t
    {
        std::string name;
        std::uintptr_t start, end;

        segment_t operator[ ]( const std::string_view segment_name ) const
        {
            return { reinterpret_cast< const void* >( start ), segment_name };
        }

        std::vector< export_t > get_exports( ) const
        {
            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( start );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( start + dos->e_lfanew );

            const auto directory_header = nt->OptionalHeader.DataDirectory [ IMAGE_DIRECTORY_ENTRY_EXPORT ];
            if ( !directory_header.VirtualAddress )
                return { };

            const auto export_dir = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( start + directory_header.VirtualAddress );
            const auto name_table = reinterpret_cast< const std::uint32_t* >( start + export_dir->AddressOfNames );
            const auto ord_table = reinterpret_cast< const std::uint16_t* >( start + export_dir->AddressOfNameOrdinals );
            const auto addr_table = reinterpret_cast< const std::uint32_t* >( start + export_dir->AddressOfFunctions );

            std::vector< export_t > exports( export_dir->NumberOfNames );

            for ( auto i = 0u; i < export_dir->NumberOfNames; ++i )
            {
                const auto name_str = reinterpret_cast< const char* >( start + name_table [ i ] );
                const auto ord = ord_table [ i ];
                const auto addr = start + addr_table [ ord ];

                exports [ i ] = { name_str, ord, addr };
            }

            return exports;
        }

        [[nodiscard]] std::vector< segment_t > get_segments( ) const
        {
            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( start );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( start + dos->e_lfanew );

            const auto section = reinterpret_cast< const IMAGE_SECTION_HEADER* >( reinterpret_cast< const std::uint8_t* >( &nt->OptionalHeader ) + nt->FileHeader.SizeOfOptionalHeader );

            std::vector< segment_t > segments;
            segments.reserve( nt->FileHeader.NumberOfSections );

            for ( auto i = 0u; i < nt->FileHeader.NumberOfSections; ++i )
            {
                const segment_t seg( dos, &section [ i ] );
                segments.push_back( seg );
            }

            return segments;
        }

        [[nodiscard]] std::vector< export_t > get_imports( ) const
        {
            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( start );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( start + dos->e_lfanew );

            const auto directory_header = &nt->OptionalHeader.DataDirectory [ IMAGE_DIRECTORY_ENTRY_IMPORT ];
            if ( !directory_header->VirtualAddress )
                return { };

            const auto import_dir = reinterpret_cast< const IMAGE_IMPORT_DESCRIPTOR* >( start + directory_header->VirtualAddress );
            std::vector< export_t > imports;

            for ( auto i = 0u;; ++i )
            {
                if ( !import_dir [ i ].OriginalFirstThunk )
                    break;

                const auto directory = &import_dir [ i ];

                const auto name_table = reinterpret_cast< const std::uint32_t* >( start + directory->OriginalFirstThunk );
                const auto addr_table = reinterpret_cast< const std::uint32_t* >( start + directory->FirstThunk );

                for ( auto j = 0u;; ++j )
                {
                    if ( !addr_table [ j ] )
                        break;

                    if ( !name_table [ j ] )
                        continue;

                    std::string_view name_str;

                    constexpr auto name_alignment = 2;

                    const auto addr = &addr_table [ j ];
                    const auto name_ptr = reinterpret_cast< const char* >( start + name_table [ j ] ) + name_alignment;

#if UD_USE_SEH
                    // using SEH here is not a very good solution
                    // however, it's faster than querying that page protection to see if it's readable
                    __try
                    {
                        name = name_ptr;
                    }
                    __except ( EXCEPTION_EXECUTE_HANDLER )
                    {
                        name = "";
                    }
#else
                    // runtime overhead of ~3us compared to SEH on single calls
                    // on bulk calls it can go up to ~300-500us
                    name_str = is_valid_page( name_ptr, PAGE_READONLY ) ? name_ptr : "";
#endif

                    // emplace_back doesn't allow for implicit conversion, so we have to do it manually
                    imports.push_back( { name_str, static_cast< std::uint16_t >( j ), reinterpret_cast< std::uintptr_t >( addr ) } );
                }
            }

            return imports;
        }

        template < typename ty = std::uintptr_t >
        ty get_address( const std::string_view name ) const
        {
            for ( const auto& export_ : get_exports( ) )
            {
                if ( export_.name.find( name ) != std::string_view::npos )
                    return ty( export_.address );
            }

            return 0;
        }

        template < typename ty = std::uintptr_t >
        std::optional< ty > find_pattern( const std::string_view pattern ) const
        {
            return find_pattern_primitive< ty >( start, end, pattern );
        }

        [[nodiscard]] std::vector< std::string_view > get_strings( const std::size_t minimum_size = 0 ) const
        {
            std::vector< std::string_view > result;

            const auto rdata = ( *this ) [ ".rdata" ];

            if ( !rdata.size )
                return { };

            const auto start = reinterpret_cast< const std::uint8_t* >( rdata.start );
            const auto end = reinterpret_cast< const std::uint8_t* >( rdata.end );

            for ( auto i = start; i < end; ++i )
            {
                if ( *i == 0 || *i > 127 )
                    continue;

                const auto str = reinterpret_cast< const char* >( i );
                const auto sz = std::strlen( str );

                if ( !sz || sz < minimum_size )
                    continue;

                result.emplace_back( str, sz );
                i += sz;
            }

            return result;
        }

        module_t( )
        {
            init( GetModuleHandle( nullptr ) );
        }

        explicit module_t( void* const handle )
        {
            init( handle );
        }

        explicit module_t( const std::string_view module_name )
        {
            init( GetModuleHandleA( module_name.data( ) ) );
        }

    private:
        void* module;

        void init( void* const handle )
        {
            module = handle;

            const auto dos = reinterpret_cast< const IMAGE_DOS_HEADER* >( handle );
            const auto nt = reinterpret_cast< const IMAGE_NT_HEADERS* >( reinterpret_cast< const std::uint8_t* >( handle ) + dos->e_lfanew );

            start = reinterpret_cast< std::uintptr_t >( handle );
            end = start + nt->OptionalHeader.SizeOfImage;

            char buffer [ MAX_PATH ];
            const auto sz = GetModuleFileNameA( static_cast< HMODULE >( handle ), buffer, MAX_PATH );

            name = sz ? std::string { buffer, sz } : std::string { };
        }
    };

    inline std::vector< module_t > get_modules( )
    {
        std::vector< module_t > result;

#if defined( _M_X64 )
        const auto peb = reinterpret_cast< const PEB* >( __readgsqword( 0x60 ) );
#else
        const auto peb = reinterpret_cast< const PEB* >( __readfsdword( 0x30 ) );
#endif

        const auto modules = reinterpret_cast< const LIST_ENTRY* >( peb->Ldr->InMemoryOrderModuleList.Flink );
        for ( auto i = modules->Flink; i != modules; i = i->Flink )
        {
            const auto entry = reinterpret_cast< const LDR_DATA_TABLE_ENTRY* >( i );

            if ( entry->Reserved2 [ 0 ] || entry->DllBase )
                result.emplace_back( entry->Reserved2 [ 0 ] ? entry->Reserved2 [ 0 ] : entry->DllBase );
        }

        return result;
    }

    inline std::optional< module_t > get_module_at_address( const std::uintptr_t address )
    {
        for ( const auto& module : get_modules( ) )
        {
            if ( module.start <= address && address < module.end )
                return module;
        }

        return std::nullopt;
    }

    inline std::optional< export_t > get_export( const std::uintptr_t address )
    {
        for ( const auto& module : get_modules( ) )
        {
            if ( module.start <= address && address < module.end )
            {
                const auto exports = module.get_exports( );
                for ( const auto& export_ : exports )
                {
                    if ( export_.address == address )
                        return export_;
                }
            }
        }

        return std::nullopt;
    }

    template < typename rel_t, typename ty = std::uintptr_t >
    ty calculate_relative( const std::uintptr_t address, const std::uint8_t size, const std::uint8_t offset )
    {
        return ty( address + *reinterpret_cast< rel_t* >( address + offset ) + size );
    }
}

template < std::size_t size >
UD_FORCEINLINE std::ostream& operator<<( std::ostream& os, const ud::details::comp_string_t< size >& str )
{
    return os << std::string_view { str.data, str.size };
}

#if defined( _MSC_VER )
#pragma warning( pop )
#endif