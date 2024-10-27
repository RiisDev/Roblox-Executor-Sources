import { Box, ScrollArea, Stack } from "@mantine/core";
import { NextSeo } from "next-seo";
import { FC, PropsWithChildren } from "react";
import { Footer } from "./Footer";
import { Navigation } from "./Navigation";

export const DocsLayout: FC<PropsWithChildren> = ({ children }) => {
  return (
    <>
      <NextSeo
        title="Hydrogen Documentation"
        description="Explore the Hydrogen documentation, filled with developer guides, helpful tips, and a full reference for our scripting API."
        openGraph={{
          type: "website",
          locale: "en_US",
          url: "https://hydrogen.sh/docs",
          site_name: "Hydrogen Documentation",
        }}
      />

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
    </>
  );
};
