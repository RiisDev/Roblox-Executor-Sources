R"(
					local function register_fn( fn, names )
						for i = 1, #names do
							local name = names[ i ];	

							getfenv( )[ name ] = coroutine.wrap( fn );
						end
					end

					register_fn( function( ) 
						return getfenv( 0 );
					end, { 'getgenv' } );

					register_fn( function( ) 
						local instances = {};

						for _,v in pairs(getgc()) do
							if ( typeof( v ) == "Instance" ) then
								table.insert( v );
							end
						end

						return instances;
					end, { 'getinstances' } );

				register_fn( function( ) 
						local instances = {};

						for _,v in pairs(getgc()) do
							if ( typeof( v ) == "Instance" and v.Parent == nil ) then
								table.insert( v );
							end
						end

						return instances;
					end, { 'getnilinstances' } );
				)"