#!/bin/sh
/bin/sh -ec 'pnpm dev:proxy' &
/bin/sh -ec 'pnpm dev:web-android' &
/bin/sh -ec 'pnpm dev:web-dashboard' &
/bin/sh -ec 'pnpm dev:web-gateway' &
/bin/sh -ec 'pnpm dev:web-landing' &
/bin/sh -ec 'pnpm dev:services-api' &
/bin/sh -ec 'pnpm dev:services-bot' &