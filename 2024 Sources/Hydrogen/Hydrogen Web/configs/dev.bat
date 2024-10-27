wt -d . --suppressApplicationTitle --title "Proxy" --tabColor #000000 cmd /k pnpm dev:proxy;^
nt -d . --suppressApplicationTitle --title "Web Android" --tabColor #61dbfa cmd /k pnpm dev:web-android;^
nt -d . --suppressApplicationTitle --title "Web Dashboard" --tabColor #61dbfa cmd /k pnpm dev:web-dashboard;^
nt -d . --suppressApplicationTitle --title "Web Gateway" --tabColor #61dbfa cmd /k pnpm dev:web-gateway;^
nt -d . --suppressApplicationTitle --title "Web Landing" --tabColor #61dbfa cmd /k pnpm dev:web-landing;^
nt -d . --suppressApplicationTitle --title "Service API" --tabColor #599c60 cmd /k pnpm dev:services-api;^
nt -d . --suppressApplicationTitle --title "Service Bot" --tabColor #5865F2 cmd /k pnpm dev:services-bot
cls