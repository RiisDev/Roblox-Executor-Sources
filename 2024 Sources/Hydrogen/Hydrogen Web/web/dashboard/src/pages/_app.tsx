import { DefaultLayout } from "components/Layouts";
import { Mantine } from "components/_app/Mantine";
import { RouterTransition } from "components/_app/RouterTransition";
import { useApi } from "hooks/useApi";
import { AppProps } from "next/app";
import { useRouter } from "next/router";
import { FC, useEffect, useState } from "react";

const App: FC<AppProps> = ({ Component, pageProps }) => {
  const { fetch } = useApi();
  const [isLoaded, setLoaded] = useState(false);
  const router = useRouter();

  useEffect(() => {
    if (!router.isReady || isLoaded) return;

    fetch({
      method: "GET",
      url: `/auth?apiKey=${(router.query as any).apiKey}`,
    }).catch(() => {
      router.push(`https://${process.env.NEXT_PUBLIC_HOSTNAME}`);
      return;
    });

    setLoaded(true);
  }, [router.isReady]);

  if (!isLoaded) return null;

  return (
    <>
      <Mantine>
        <RouterTransition />
        <DefaultLayout>
          <Component {...pageProps} />
        </DefaultLayout>
      </Mantine>
    </>
  );
};

export default App;
