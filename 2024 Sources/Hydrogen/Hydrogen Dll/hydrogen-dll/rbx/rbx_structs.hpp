#pragma once

/*
	For Roblox Employees:
	We reverse engineering your softwares
	is not copied from any copyrighted materials at all
	No Liabilities Disclosed.
*/

#include <atomic>
#include <dependencies/luau/lstate.h>

namespace rbx
{
	namespace refs
	{
		class LiveThreadRef
		{
		public:
			void* refs;

			lua_State* L;
			int threadId;

			LiveThreadRef( lua_State* thread )
				:L( thread )
			{
				lua_pushthread( L );
				threadId = lua_ref( L, -1 );
				lua_pop( L, 1 );
			}

			~LiveThreadRef( )
			{
				lua_unref( L, threadId );
			}

			bool empty( ) const
			{
				return L == nullptr;
			}
			lua_State* thread( ) const
			{
				return L;
			}

		};

		class ThreadRef
		{
		public:
			LiveThreadRef* liveThreadRef;
			friend class WeakThreadRef;
			ThreadRef( LiveThreadRef* liveThreadRef ) :liveThreadRef( liveThreadRef ) { }

			ThreadRef( lua_State* thread )
				:liveThreadRef( new LiveThreadRef( thread ) )
			{
			}
			lua_State* get( ) const
			{
				return liveThreadRef ? liveThreadRef->thread( ) : NULL;
			}
			operator lua_State* ( ) const
			{
				return get( );
			}
			bool empty( ) const
			{
				return liveThreadRef && liveThreadRef->thread( ) != NULL;
			}
		};

		class WeakThreadRef
		{
			void* refs;
			void* mutex;

		public:
			class Node
			{
				void* refs;
			public:
				friend class WeakThreadRef;
				WeakThreadRef* first;

				Node( ) :first( 0 ) { }

				template<class Func>
				void forEachRefs( Func func )
				{
					for ( WeakThreadRef* ref = first; ref != NULL; ref = ref->next )
					{
						std::printf( "%p\n", ref );

						//func( ref->lock( ) );
					}
				}
			};
			friend class Node;

			WeakThreadRef* previous;
			WeakThreadRef* next;
			LiveThreadRef* liveThreadRef;

			Node* node;
			lua_State* thread( ) const
			{
				return threadDangerous( );
			}

			bool empty( ) const
			{
				return liveThreadRef ? liveThreadRef->thread( ) == 0 : true;
			}
			ThreadRef lock( )
			{
				return ThreadRef( liveThreadRef );
			}
			lua_State* threadDangerous( ) const
			{
				return liveThreadRef ? liveThreadRef->thread( ) : NULL;
			}

		};

		class WeakFunctionRef : public WeakThreadRef
		{
		public:
			int functionId;
			typedef WeakThreadRef Super;

			WeakFunctionRef( ) :functionId( 0 ) { }
		protected:
			virtual void removeRef( ) { };
		};
	}


	template<class Item, class Tag = Item>
	class Set
	{
	private:
		class NextRef
		{
			friend class Set;
		protected:
			Item* next;
			inline NextRef( ) throw( )
				:next( 0 )
			{
			}
			inline NextRef( const NextRef& other ) throw( )
				:next( 0 )
			{
				// Copies of objects aren't automatically added to containers
			}
			inline NextRef& operator=( const NextRef& other ) throw( )
			{
				// This object retains its membership to its container
			}
		};
	public:
		class Hook : public NextRef
		{
			friend class Set;
			Set* _container;
			NextRef* prev;
		public:
			inline Hook( ) throw( )
				:_container( 0 ), prev( 0 )
			{
			}
			inline Hook( const Hook& other ) throw( )
				:_container( 0 ), prev( 0 )
			{
				// Copies of objects aren't automatically added to containers
			}
			inline Hook& operator=( const Hook& other ) throw( )
			{
				// This object retains its membership to its container
			}
			inline ~Hook( ) throw( )
			{
				remove( );
			}
			inline void remove( ) throw( )
			{
				if ( is_linked( ) )
				{

					if ( prev )
						prev->Set::NextRef::next = NextRef::next;
					if ( NextRef::next )
						NextRef::next->Set::Hook::prev = prev;

					_container->count--;

					NextRef::next = 0;
					prev = 0;
					_container = 0;
				}
			}
			inline bool is_linked( ) const throw( )
			{

				return _container != 0;
			}
			inline Set* container( ) throw( )
			{
				return _container;
			}
		};

		class Iterator
		{
			friend class Set;
			Item* item;
			Iterator( Item* item ) throw( )
				:item( item )
			{

			}
		public:
			inline Iterator( ) throw( )
				:item( 0 )
			{
			}

			inline bool operator==( const Iterator& other ) const throw( )
			{
				return item == other.item;
			}

			inline bool operator!=( const Iterator& other ) const throw( )
			{
				return item != other.item;
			}

			inline Item* operator->( ) throw( )
			{
				return item;
			}

			inline Item& operator*( )  throw( )
			{
				return *item;
			}

			inline Iterator& operator++( ) throw( )
			{

				item = item->Set::Hook::next;

				return *this;
			}

			inline bool empty( ) const throw( )
			{
				return item == 0;
			}

			// for std iterators:
			typedef std::forward_iterator_tag iterator_category;
			typedef Item& value_type;
			typedef void difference_type;
			typedef /*typename*/ Item* pointer;
			typedef /*typename*/ Item& reference;
		};

		inline Set( ) throw( )
			:count( 0 )
		{
		}

		inline ~Set( ) throw( )
		{
			for ( Iterator iter = begin( ); !iter.empty( ); iter = erase( iter ) )
				;
		}

		inline size_t size( ) const throw( ) { return count; }
		inline bool empty( ) const throw( ) { return count == 0; }

		Iterator erase( Iterator iter ) throw( )
		{
			Item& item( *iter );
			++iter;
			remove_element( item );
			return iter;
		}

		bool remove_element( Item& item ) throw( )
		{
			if ( item.Set::Hook::_container == this )
			{
				item.Set::Hook::remove( );
				return true;
			}
			else
				return false;
		}

		void insert( Item& item ) throw( )
		{
			if ( item.Set::Hook::_container == this )
				return;

			// Items can be in only one list at a time
			item.Set::Hook::remove( );

			Item* head = head_ref.next;
			if ( head )
			{
				item.Set::NextRef::next = head;
				head->Set::Hook::prev = &item;
			}
			head_ref.next = &item;
			item.Set::Hook::prev = &head_ref;

			item.Set::Hook::_container = this;

			count++;
		}

		inline Iterator begin( ) throw( )
		{
			return Iterator( head_ref.next );
		}

		inline Iterator end( ) throw( )
		{
			return Iterator( );
		}

		// For the std iterator pattern:
		typedef Iterator iterator;

		// For the boost::intrusive pattern:
		inline void push_front( Item& item ) throw( )
		{
			insert( item );
		}

		// For the boost::intrusive pattern:
		inline Iterator iterator_to( Item& item ) throw( )
		{
			return item.Set::Hook::_container == this ? Iterator( &item ) : Iterator( );
		}

	private:
		size_t count;
		NextRef head_ref;
	};

	class RobloxExtraSpace : public Set<RobloxExtraSpace>::Hook
	{
		typedef Set<RobloxExtraSpace> AllThreads;

		struct Shared
		{
			int threadCount;
			void* context;
			// We need to keep track of all Nodes so that we can clear them on shutdown.
			// See eraseRefsFromAllNodes
			AllThreads allThreads;
			Shared( ) :threadCount( 0 ), context( NULL ) { }
		};
		const std::shared_ptr<Shared> shared;
		typedef refs::WeakThreadRef::Node Node;
		Node* node;
	public:
		std::uint32_t identity : 5;
		bool yieldCaptured : 1;
		std::weak_ptr<void*> script;	// The script associated with this thread, if any
		void* continuations;

		size_t getThreadCount( ) const { return ( size_t )shared->threadCount; }

		void createNewNode( )
		{
			node = new Node( );
		}

		template<class Func>
		inline void forEachThread( Func func )
		{
			for ( AllThreads::Iterator iter = shared->allThreads.begin( ); iter != shared->allThreads.end( ); ++iter )
				iter->node->forEachRefs( func );
		}
	};
}