import { Stack, Text, ThemeIcon } from "@mantine/core";
import { TablerIcon } from "@tabler/icons";
import { FC } from "react";

type Props = {
  icon: TablerIcon;
  title: string;
  description: string;
};

export const FeatureCard: FC<Props> = ({ icon: Icon, title, description }) => (
  <Stack>
    <ThemeIcon radius="xl" size={40} variant="light">
      <Icon />
    </ThemeIcon>
    <Text>{title}</Text>
    <Text size="sm" color="dimmed">
      {description}
    </Text>
  </Stack>
);
