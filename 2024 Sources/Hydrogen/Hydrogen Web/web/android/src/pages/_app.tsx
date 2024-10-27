import { DndContext } from "@dnd-kit/core";
import { DefaultLayout } from "components/Layouts";
import { Mantine } from "components/_app/Mantine";
import { AppProps } from "next/app";
import { FC } from "react";

const App: FC<AppProps> = ({ Component, pageProps }) => (
  <Mantine>
    <DndContext>
      <DefaultLayout>
        <Component {...pageProps} />
      </DefaultLayout>
    </DndContext>
  </Mantine>
);

export default App;
