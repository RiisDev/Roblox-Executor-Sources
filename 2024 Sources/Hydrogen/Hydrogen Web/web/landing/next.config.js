/** @type {import('next').NextConfig} */
const nextConfig = {
  swcMinify: false,
  poweredByHeader: false,
  reactStrictMode: true,
  redirects: async () => [
    {
      source: "/community",
      destination: `https://api.${process.env.NEXT_PUBLIC_HOSTNAME}/discord`,
      permanent: true,
    },
    {
      source: "/download/android",
      destination: "https://r.work.ink/2zT/hydrogen-android",
      permanent: false,
    },
    {
      source: "/download/macos",
      destination: "https://cdn.hydrogen.sh/Hydrogen_MacOS.zip",
      permanent: false,
    },
  ],
};

module.exports = nextConfig;
