import { MantineProvider } from "@mantine/core";
import { ModalsProvider } from "@mantine/modals";
import { NotificationsProvider } from "@mantine/notifications";
import { FC, PropsWithChildren } from "react";

export const Mantine: FC<PropsWithChildren> = ({ children }) => (
  <MantineProvider
    withGlobalStyles
    withNormalizeCSS
    theme={{
      colorScheme: "dark",
      fontFamily: "Inter",
      headings: {
        fontFamily: "Inter",
      },
      components: {
        Modal: {
          defaultProps: {
            exitTransitionDuration: 60,
          },
        },
      },
    }}
  >
    <ModalsProvider>
      <NotificationsProvider>{children}</NotificationsProvider>
    </ModalsProvider>
  </MantineProvider>
);
