import { Group } from "@mantine/core";
import { AuthDialog } from "components/Dialogs/AuthDialog";
import { Logo } from "components/Logo";
import { FC } from "react";
import { Legal } from "../../Legal";

export const Footer: FC = () => {
  return (
    <Group
      py="lg"
      mx="lg"
      position="apart"
      sx={{
        borderTop: "1px solid #27272a",
      }}
    >
      <Logo />

      <Group spacing="lg">
        <Legal />
        <AuthDialog />
      </Group>
    </Group>
  );
};
