import { Box, ScrollArea, Stack } from "@mantine/core";
import { FC, PropsWithChildren } from "react";
import { Footer } from "./Footer";
import { Navigation } from "./Navigation";

export const LandingLayout: FC<PropsWithChildren> = ({ children }) => {
  return (
    <ScrollArea
      styles={{
        root: {
          height: "100vh",
        },
        scrollbar: {
          zIndex: 4,
        },
      }}
    >
      <Stack
        spacing={0}
        sx={(theme) => ({
          color: theme.white,
          minHeight: "100vh",
          backgroundImage: theme.fn.linearGradient(24, "#18181b", "#000"),
        })}
      >
        <Navigation />
        <Box
          sx={{
            display: "flex",
            flex: "1 0 auto",
          }}
        >
          {children}
        </Box>
        <Footer />
      </Stack>
    </ScrollArea>
  );
};
