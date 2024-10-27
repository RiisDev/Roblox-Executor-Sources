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
      defaultGradient: {
        from: "#4ba9ff",
        to: "#fff",
        deg: 175,
      },
      components: {
        Button: {
          defaultProps: {
            radius: "xl",
          },
        },
        TextInput: {
          defaultProps: {
            radius: "xl",
          },
        },
        Accordion: {
          styles: {
            control: {
              paddingTop: 20,
              paddingBottom: 20,
            },
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
