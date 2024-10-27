import { Group, MantineColor, Title } from "@mantine/core";
import { IconDroplet } from "@tabler/icons";
import { FC } from "react";

type Props = {
  color?: MantineColor;
};

export const Logo: FC<Props> = ({ color }) => {
  return (
    <Group
      spacing={2}
      sx={{
        userSelect: "none",
      }}
    >
      <IconDroplet color={color || "white"} />
      <Title
        sx={{
          color: color || "white",
          fontWeight: 500,
          fontSize: 20,
          whiteSpace: "nowrap",
        }}
      >
        Hydrogen
      </Title>
    </Group>
  );
};
