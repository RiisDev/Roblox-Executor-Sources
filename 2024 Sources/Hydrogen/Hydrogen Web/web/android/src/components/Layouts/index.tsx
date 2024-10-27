import { Box, ScrollArea, Stack } from "@mantine/core";
import { FC, PropsWithChildren } from "react";

export const DefaultLayout: FC<PropsWithChildren> = ({ children }) => {
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
          backgroundColor: "#000",
        })}
      >
        <Box
          sx={{
            display: "flex",
            flex: "1 0 auto",
          }}
        >
          {children}
        </Box>
      </Stack>
    </ScrollArea>
  );
};
