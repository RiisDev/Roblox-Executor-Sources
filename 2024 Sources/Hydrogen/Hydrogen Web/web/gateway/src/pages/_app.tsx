import { Analytics } from "@vercel/analytics/react";
import { DefaultLayout } from "components/Layouts";
import { Mantine } from "components/_app/Mantine";
import { RouterTransition } from "components/_app/RouterTransition";
import { DefaultSeo } from "next-seo";
import { AppProps } from "next/app";
import { FC } from "react";

const App: FC<AppProps> = ({ Component, pageProps }) => (
  <>
    <DefaultSeo
      title="Hydrogen"
      description="A huge step forward in script execution, now available on Android & macOS."
      openGraph={{
        type: "website",
        locale: "en_US",
        url: "https://hydrogen.sh",
        site_name: "Hydrogen",
        images: [
          {
            url: "/icon.png",
            alt: "Icon",
            type: "image/png",
          },
        ],
      }}
      additionalMetaTags={[
        {
          name: "theme-color",
          content: "#8cc1f1",
        },
      ]}
    />

    <Mantine>
      <RouterTransition />
      <DefaultLayout>
        <Component {...pageProps} />
        <Analytics />
      </DefaultLayout>
    </Mantine>
  </>
);

export default App;
