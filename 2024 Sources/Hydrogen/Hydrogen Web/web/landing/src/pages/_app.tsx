import { Analytics } from "@vercel/analytics/react";
import { DocsLayout } from "components/Layouts/Docs";
import { LandingLayout } from "components/Layouts/Info";
import { Parent } from "components/Parent";
import { Mantine } from "components/_app/Mantine";
import { RouterTransition } from "components/_app/RouterTransition";
import { DefaultSeo } from "next-seo";
import { AppProps } from "next/app";
import { useRouter } from "next/router";
import { FC } from "react";

const App: FC<AppProps> = ({ Component, pageProps }) => {
  const router = useRouter();

  return (
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

        <Parent
          parent={(children) =>
            router.pathname.includes("docs") ? (
              <DocsLayout>{children}</DocsLayout>
            ) : (
              <LandingLayout>{children}</LandingLayout>
            )
          }
        >
          <Component {...pageProps} />
          <Analytics />
        </Parent>
      </Mantine>
    </>
  );
};

export default App;
